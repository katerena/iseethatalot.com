#!/usr/bin/env python

#builtins
import logging
import os
import tempfile
import time
import ConfigParser
import saver
import unicodedata
import re
import hashlib
from cStringIO import StringIO

#ain't no party like a third party
import MySQLdb

#get the all powerful alot maker
from alot_maker import AlotMaker, ProcessError

log = logging.getLogger("alot_main")

MAX_STATUS_LEN = 90

class TempImageFile(object):
    """temporary file object context.  Deletes the file
       on the way out.  convinient!
    """

    def __init__(self, extension):
        self.extension = extension
        self.file_path = None
        self.file_handle = None

    def __enter__(self):
        """on entering the context, create a temp file, save the path so we can
           delete it, and return the file handle and path.
        """
        os_file, self.file_path = tempfile.mkstemp(suffix=self.extension)
        self.file_handle = os.fdopen(os_file, 'wb')
        return self.file_handle, self.file_path

    def __exit__(self, exc_type, value, traceback):
        """close and delete the file.
        """
        self.file_handle.close()
        os.unlink(self.file_path)

def update_status(db_conn, alot_id, processed, status, warning=None):
    if warning:
        log.warn(warning)
    try:
        status = status[:MAX_STATUS_LEN]
        c = db_conn.cursor()
        c.execute("""UPDATE alot
                   SET processed=%s, status=%s
                   WHERE id=%s""",
                  (processed, status, alot_id))
        c.close()
    except Exception, e:
        log.error("Error updating status: %s", str(e))


def run_forever(db_conn, maker, saver):
    """ridiculously-unscalble solution.  Reads off the db to see what's not been
        processed, and runs those.  Note that you should only run one of these
        at a time to avoid having more than one machine pick up the same work.
        booooo.  move to a proper task queue system when you outgrow this.

        The end result of this is a bit cumbersome.  There are a few possible
        states:
        1. processed=True, status is NULL - everything went well!
        2. processed=False, status is NOT NULL - some unknown exception
          kept us from processing this.  The details will be in the logs
          provided you're logging at exception level or lower, and the message
          will be in the status field.  Try again later, maybe?
          Haven't figured out a set number of auto-retrys yet.
        3. processed=True, status is NOT NULL - we couldn't download the
          image, or the image couldn't be processed, or it's not an image.
          Show the user the error message in the status field and don't try
          again automatically.  This hypothetically could be in error IF the
          remote server was down, but that's a rare case and we wouldn't want
          to retry automatically, anyway.
    """

    log.info("monitoring alot of alots...")
    while True:

        #check for unprocessed stuffs which have no error messages
        c = db_conn.cursor()
        c.execute("""SELECT id, image, added, word
                    FROM alot WHERE composed_url IS NULL
                        AND processed = FALSE
                        AND status IS NULL
                    ORDER BY added ASC""")
        results = c.fetchall()
        c.close()

        log.debug("found %s to process", len(results))

        if len(results):

            # we have some to process, run through them
            # in FIFO order.
            for r in results:
                alot_id = r[0]
                base_image_url = r[1]
                date_added = r[2]
                alot_word = r[3]

                log.info("processing #%s, alot of %s, at %s", str(alot_id), alot_word, base_image_url)

                try:
                    #call out to download and resize the image
                    alot_image = maker.process(base_image_url, alot_word)

                    image_file = StringIO()
                    alot_image.save(image_file, format="PNG")
                    file_contents = image_file.getvalue()

                    # save the image somewhere
                    hash_str = hashlib.sha1(file_contents).hexdigest()[0:8]
                    alot_path = "alot-of-%s-%d-%s.png" %(slugify(alot_word), alot_id, hash_str)

                    alot_url = saver.save_png(alot_path, file_contents)

                except ProcessError as e:
                    #we had a handled error, set the error message to the
                    #exception message and flag it as processed.
                    update_status(db_conn, alot_id, processed=True, status=str(e),
                                  warning="caught handled error: %s" % str(e))

                except Exception as e:
                    #catch all other exceptions, log them.
                    #set it to unprocessed, but an error message, so we don't
                    #immediately process it again.
                    update_status(db_conn, alot_id, processed=False, status=str(e), 
                                  warning="unknown exception in image processor: %s" % str(e))

                else:
                    #no exception, set it to processed and add on the data
                    log.info("successfully processed")

                    # point the db at the image
                    c = db_conn.cursor()
                    c.execute("""UPDATE alot
                                     SET processed=TRUE, 
                                        composed_url=%s,
                                        composed_path=%s
                                     WHERE id=%s""",
                              (alot_url, alot_path, alot_id))
                    c.close()

                db_conn.commit()

        #give it one second so we don't thrash the system.
        time.sleep(1)


class QuoteConfigParser(ConfigParser.ConfigParser):
    def get(self, section, option, default=None):
        if not self.has_option(section, option):
            return default
        else:
            val = ConfigParser.ConfigParser.get(self, section, option)
            if val[0] == '"' and val[len(val) - 1] == '"':
                return val.strip('"')
            elif val[0] == "'" and val[len(val) - 1] == "'":
                return val.strip("'")
            else:
                return val

def slugify(value):
    """
    Normalizes string, converts to lowercase, removes non-alpha characters,
    and converts spaces to hyphens.
    """
    value = unicode(value)
    value = unicodedata.normalize('NFKD', value).encode('ascii', 'ignore')
    value = unicode(re.sub('[^\w\s-]', '', value).strip().lower())
    return re.sub('[-\s]+', '-', value)


if __name__ == '__main__':
    # pop up a level
    os.chdir('..');

    #read our configs
    conf = QuoteConfigParser()
    conf.read('config/app.ini')

    #set up logging at level default as the, uh, default.
    loglevel = conf.get('main', 'log_level')
    loglevel = getattr(logging, loglevel.upper(), None)
    logging.basicConfig(level=loglevel)

    # configure the alot maker
    maker = AlotMaker(
        conf.get("maker", "overlay"),
        conf.get("maker", "font"),
        conf.getint("maker", "tiles_across"),
        conf.get("maker", "logo_text"),
        conf.getint("maker", "max_tile_size"))

    # connect to the database
    conn = MySQLdb.connect(
        host=conf.get("db", "host"),
        port=conf.getint("db", "port"),
        db=conf.get("db", "database"),
        user=conf.get("db", "username"),
        passwd=conf.get("db", "password"))

    # without this our repetitious queries give the same results over and over
    conn.autocommit(True)

    # we save the images on Amazon S3
    bucket_name = conf.get("saver", "bucket")
    access_key = conf.get("saver", "aws_access_key_id")
    secret_key = conf.get("saver", "aws_secret_access_key")
    if access_key:
        saver = saver.S3_saver(access_key, secret_key, bucket_name)
    else:
        saver = saver.Disk_saver(bucket_name)

    #go go go
    run_forever(conn, maker, saver)


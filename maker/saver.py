import os
from boto.s3.connection import S3Connection
from boto.s3.key import Key

import logging
log = logging.getLogger("s3_saver")

class S3_saver(object):

    def __init__(self, access_key, secret_key, bucket_name):
        log.info('Connecting to Amazon S3 bucket %s' %(bucket_name))
        self.conn = S3Connection(access_key, secret_key)
        self.bucket = self.conn.get_bucket(bucket_name)

    def save_png(self, keyString, valueString):

        key = Key(self.bucket)
        key.key = keyString
        key.set_metadata('Content-Type', 'image/png')
        key.set_contents_from_string(valueString)
        key.make_public()

        # a non-expiring URL
        return key.generate_url(expires_in=0, query_auth=False)

class Disk_saver(object):

    def __init__(self, bucket):
        log.info('Making alot directory')

        self.diskPath = 'web/tmp/%s' %(bucket)
        self.urlPrefix = 'tmp/%s' %(bucket)

        try:
            os.makedirs('web/tmp/%s' %(bucket))
        except os.error:
            log.info("Directory already exists. Continuing.")

    def save_png(self, keyString, valueString):
        url = self.urlPrefix + '/' + keyString
        path = self.diskPath + '/' + keyString

        with open(path, 'wb') as output:
            output.write(valueString)

        return url
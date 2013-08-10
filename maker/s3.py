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


#!/usr/bin/env python

import os
import glob
import hashlib
import datetime

# Go through the static files in web and create a version map
patterns = [
    'css/styles.css',
    'js/main.js',
    'img/alots/*.png'
]
map_filename = '../config/map.ini'

# go to the web directory
os.chdir('../web')

files_to_map = []
for pattern in patterns:
    files = glob.glob(pattern)
    files_to_map.extend(files)

files_to_map = [f.replace('\\', '/') for f in files_to_map]

with open(map_filename, 'w') as map_file:
    now = datetime.datetime.now()
    map_file.write('; Map created %s\n' %(now.strftime("%Y-%m-%d %H:%M")))
    for filename in files_to_map:
        with open(filename, 'rb') as infile:
            result = hashlib.sha1(infile.read()).hexdigest()[0:8]
            map_file.write("%s = %s?%s\n" %(filename, filename, result))

print "Mapped %d files to %s." %(len(files_to_map), map_filename)

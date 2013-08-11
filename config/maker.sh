#!/bin/bash

# Script for preparing the maker to run on Amazon EC2

# install system libs
sudo yum -y install python-devel freetype-devel libjpeg-devel libtiff-devel

# install python utils
sudo easy_install pip supervisor
sudo pip install virtualenv

# create and load the virtual env
cd /var/app/current/maker
virtualenv --distribute venv
source venv/bin/activate

# install python packages
pip install -r requirements.txt

# make sure we have a fresh config
supervisorctl -c ../config/supervisor.conf shutdown

# set up the supervisor daemon
supervisord -c ../config/supervisor.conf

# restart the maker process
sudo touch /tmp/alot_maker.log
supervisorctl -c ../config/supervisor.conf restart alot_maker
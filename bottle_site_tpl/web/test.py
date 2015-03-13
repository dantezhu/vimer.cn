#!/usr/bin/python
# -*- coding: utf-8 -*-

from bottle import Bottle
from bottle import debug, run
from bottle import view
from bottle import redirect, abort, static_file
from bottle import jinja2_template as template
from bottle import request, response, local

app = Bottle()

@app.route('/')
def index():
    return template('test', name='dantezhu')

if __name__ == '__main__':
    debug(True)
    run(app, reloader=True)

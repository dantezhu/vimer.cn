#!/usr/bin/python
# -*- coding: utf-8 -*-

import httplib
import urllib

def https_send(ip, url_path, params, method='GET'):

    ec_params = urllib.urlencode(params)

    conn = httplib.HTTPSConnection(ip)

    method = method.upper()

    if method == 'GET':
        url = '%s?%s' % (url_path, ec_params)
        conn.request(method, url)
    else:
        conn.request(method, url_path, ec_params)

    rsp = conn.getresponse()

    if rsp.status != 200:
        raise ValueError, 'status:%d' % rsp.status
    data = rsp.read()

    return data

#!/usr/bin/python
# -*- coding: utf-8 -*-

from wtforms import Form, BooleanField, TextField, PasswordField, validators

class RegForm(Form):
    '''注册'''

    email = TextField(u'邮箱', [validators.Length(min=6, max=120, message=u'邮箱最少为6位'), validators.Email(message=u'请填写正确的邮箱地址')])
    password = PasswordField(u'密码', [validators.Length(min=6, max=120, message=u'密码最少为6位')])
    password_second = PasswordField(u'密码（二次确认）', [validators.Length(min=6, max=120, message=u'密码最少为6位')])

class LoginForm(Form):
    '''登录'''

    email = TextField(u'邮箱', [validators.Length(min=6, max=120, message=u'邮箱最少为6位'), validators.Email(message=u'请填写正确的邮箱地址')])
    password = PasswordField(u'密码', [validators.Length(min=6, max=120, message=u'密码最少为6位')])

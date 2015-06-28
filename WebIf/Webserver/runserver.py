#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
from angular_flask import app


def runserver():
    port = int(os.environ.get('PORT', 80))
    app.run(host='0.0.0.0', port=port)

if __name__ == '__main__':
    runserver()

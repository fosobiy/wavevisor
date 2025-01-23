#import sys

try:
    from cStringIO import StringIO as BytesIO
except ImportError:
    from io import BytesIO

import pycdlib

iso = pycdlib.PyCdlib()
iso.new(joliet=3)

ifh1 = open('examplebin', 'rb')
foostr1 = ifh1.read()

iso.add_fp(BytesIO(foostr1), len(foostr1), '/EXAMBIN.BIN;1', joliet_path='/examplebin')

iso.write('examplec.iso')
iso.close()














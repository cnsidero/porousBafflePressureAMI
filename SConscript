# -*- mode: python -*-

import os
import utils
from build import process_lninclude
Import('env')

subdirs = Split("""
src/libraries/fvPatchFields
""")

for d in subdirs:
    lninclude = list(process_lninclude(env, d))
    SConscript(os.path.join(d,'SConscript'),
               exports=['env','lninclude'])

# Copyright (C) 2017 Applied CCM
# -*- mode: python -*-

import os
import utils
from variables import caelus_vars, init_dependent_vars
from compiler import update_compiler_settings

### Initialize toolsets based on operating system
ostype = utils.ostype()
tools = ['default']
if ostype == 'windows':
    tools += ['mingw']

### Base SCons environment
env = Environment(variables = caelus_vars,
                  tools = tools,
                  ENV = os.environ)
Help(caelus_vars.GenerateHelpText(env))
init_dependent_vars(env)
update_compiler_settings(env)

### Isolate build environments based on build options
build_dir = os.path.join(
    Dir("#").abspath,"build",env['BUILD_OPTION'])

### Main entry points
src = Split("""
.""")

for d in src:
    SConscript('%s/SConscript'%d,
               exports=['env'],
               src_dir=Dir("#").srcnode().abspath,
               variant_dir=build_dir)

### Remove build directory when cleaning
Clean(".", build_dir)

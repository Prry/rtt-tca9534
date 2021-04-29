from building import *
Import('rtconfig')

src   = []
cwd   = GetCurrentDir()

# add tca9534 src files.
if GetDepend('PKG_USING_TCA9534'):
    src += Glob('src/tca9534.c')

if GetDepend('PKG_USING_TCA9534_SAMPLE'):
    src += Glob('examples/tca9534_sample.c')

# add tca9534 include path.
path  = [cwd + '/inc']

# add src and include to group.
group = DefineGroup('tca9534', src, depend = ['PKG_USING_TCA9534'], CPPPATH = path)

Return('group')

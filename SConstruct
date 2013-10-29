LibPaths = []

env = Environment(CPPPATH=["."],
                  CXXFLAGS=['-O2', '-mfpmath=sse', '-msse4', '-march=native',
                            '-Wall', '-g', '-std=c++0x'],
                  LIBS=['c'],
                  LIBPATH=LibPaths,
                  LINKFLAGS=['-pthread'])

for p in Glob("tests/*.cpp"):
    prog = env.Program(p)
    Default(prog)

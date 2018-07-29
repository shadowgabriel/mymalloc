import os
import sys
sys.path.insert(0, 'e:/Workspace/Spectrum_analyzer/simulation/')
from GCCsupport import GCCsupport

path_exe = "maltest"
c_files = ["main.c", "mymalloc.c"]
h_files = ["mymalloc.h"]
gcc = GCCsupport(c_files, h_files, path_exe)
gcc.compile();
os.system(path_exe)
exit()
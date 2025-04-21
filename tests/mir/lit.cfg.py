# -*- Python -*-

# Configuration file for the 'lit' test runner.

import platform

import lit.formats
# Global instance of LLVMConfig provided by lit
from lit.llvm import llvm_config
from lit.llvm.subst import ToolSubst

# name: The name of this test suite.
# (config is an instance of TestingConfig created when discovering tests)
config.name = 'LIT_TEST'

# testFormat: The test format to use to interpret tests.
# As per shtest.py (my formatting):
#   ShTest is a format with one file per test. This is the primary format for
#   regression tests (...)
config.test_format = lit.formats.ShTest('0')

# suffixes: A list of file extensions to treat as test files. This is overriden
# by individual lit.local.cfg files in the test subdirectories.
config.suffixes = ['.ll','.ir']

# test_source_root: The root path where tests are located.
config.test_source_root = os.path.dirname(__file__)

# excludes: A list of directories to exclude from the testsuite. The 'Inputs'
# subdirectories contain auxiliary inputs for various tests in their parent
# directories.
config.excludes = ['Inputs']

if platform.system() == 'Darwin':
    tool_substitutions = [
        ToolSubst('%clang', "clang",
                  extra_args=["-isysroot",
                              "`xcrun --show-sdk-path`",
                              "-mlinker-version=0"]),
    ]
else:
    tool_substitutions = [
        ToolSubst('%clang', "clang",
                 )
    ]

# jf_fce by Jakub Frýz (xfryzj01)

from pprint import pprint
import sys

from jf_glob import *


def dump(var, name='variable', color=32, debug=DEBUG_DUMP):
	if debug:
		print("\033[1;" + str(color) + "m" + str(name) + ":\033[0m " + str(type(var)))
		pprint(var)

def error(error=1, msg=None, debug=DEBUG_ERROR):
	if msg is not None and debug:
		sys.stderr.write('\033[1;31mERROR:\033[0m ' + msg + '\n')
	sys.exit(error)

def spaces(times=1, debug=DEBUG_SPACES):
	if debug:
		while(times != 0):
			print('')
			times -= 1

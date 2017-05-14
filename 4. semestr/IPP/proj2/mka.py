# mka by Jakub Frýz (xfryzj01)

import argparse
import io
import os
import re
import sys

from jf_fce import *
from jf_cls import *
from jf_glob import *


# -------------------------------------------------------------------------------------------------
#	argumenty
# -------------------------------------------------------------------------------------------------

parser = argparse.ArgumentParser(description='validace a případná minimalizace dobře specifikovaného konečného automatu', add_help=False)
parser.add_argument('--help', help='nápověda', action='store_true')
parser.add_argument('--input', help='vstupní soubor', type=str, metavar='FILENAME')
parser.add_argument('--output', help='výstupní soubor', type=str, metavar='FILENAME')
parser.add_argument('-f', '--find-non-finishing', help='hledání neukončujících stavů', action='store_true')
parser.add_argument('-m', '--minimize', help='provedení minimalizace automatu', action='store_true')
parser.add_argument('-i', '--case-insensitive', help='nebude brán ohled na velikost znaků', action='store_true')

args = parser.parse_args()

# -------------------------------------------------------------------------------------------------
#	vstup
# -------------------------------------------------------------------------------------------------

if args.help:
	if len(sys.argv) == 2:
		parser.print_help()
		sys.exit(0)
	else:
		error(1, 'byla zadána špatná kombinace argumentů')

if args.minimize and args.find_non_finishing:
	error(1, 'byla zadána špatná kombinace argumentů')

input = str()
if args.input is not None:
	try:
		with io.open(args.input, 'r', encoding='utf8') as file:
			input = file.read()
	except:
		error(2, 'chyba vstupního souboru')
else:
	for line in sys.stdin:
		input += line

if args.case_insensitive:
	input = input.lower()

# -------------------------------------------------------------------------------------------------
#	vytvoření a zpracování automatu
# -------------------------------------------------------------------------------------------------

mka = Machine()
mka.buffer = re.sub(r'#.*', '', input)							# odstranění komentářů
if len(mka.buffer) == 0:
	exit(60)

mka.parse_ka()
mka.dump()
mka.check_ka()
mka.find_unreachable()
trap = mka.find_non_finishing()
if len(trap) > 1:
	error(60, 'příliš neukončujících stavů')

# -------------------------------------------------------------------------------------------------
#	provedení argumenty specifikovaných příkazů
# -------------------------------------------------------------------------------------------------

output = str()
if args.find_non_finishing:
	if len(trap) == 1:
		# sys.stdout.write(trap[0])
		# print(trap[0], end='')
		output = trap[0]
	else:
		output = str(0)
		# print('0')
	# sys.exit(0)
elif args.minimize:
	min = mka.minimize()
	min.dump()
	output = min.format()
else:
	output = mka.format()

# -------------------------------------------------------------------------------------------------
#	výstup
# -------------------------------------------------------------------------------------------------

if args.output is not None:
	try:
		with io.open(args.output, 'w', encoding='utf8') as file:
			file.write(output)
	except:
		error(3, 'chyba výstupního souboru')
else:
	sys.stdout.write(output)

sys.exit(0)

# jf_cls by Jakub Frýz (xfryzj01)

from operator import itemgetter
import copy
import sys

from jf_fce import *
from jf_glob import *


class Machine:
	ka_states = list()												# množina stavů
	ka_alphabet = list()											# vstupní abeceda
	ka_rules = list()												# množina pravidel
	ka_state_start = str()											# počáteční stav
	ka_states_end = list()											# množina koncových stavů

	buffer = ''
	buffer_id = -1

	brackets_round = 0												# musí být 2 na konci
	brackets_curly = 0												# musí být 8 na konci
	commas = 0														# musí být 4 na konci
	state = 0

# -------------------------------------------------------------------------------------------------
# 								DUMP
# -------------------------------------------------------------------------------------------------

	def dump(self, debug=DEBUG):
		if debug:
			print('')
			print('-------------------------')
			print('----------DEBUG----------')
			print('-------------------------')
			dump(self.ka_states, 'množina stavů', 33, True)
			dump(self.ka_alphabet, 'vstupní abeceda', 33, True)
			dump(self.ka_rules, 'množina pravidel', 33, True)
			dump(self.ka_state_start, 'počáteční stav', 33, True)
			dump(self.ka_states_end, 'množina koncových stavů', 33, True)
			print('-------------------------')
			print('-------------------------')
			dump(self.state, 'stav', 33, True)
			dump(self.brackets_round, 'závorky ()', 33, True)
			dump(self.brackets_curly, 'závorky {}', 33, True)
			dump(self.commas, 'čárky', 33, True)
			print('-------------------------')
			print('-----------END-----------')
			print('-------------------------')
			print('')

# -------------------------------------------------------------------------------------------------
# 								GENERÁTOR
# -------------------------------------------------------------------------------------------------

	def char_gen(self):
		while (self.buffer_id < len(self.buffer) -1):
			self.buffer_id += 1
			yield self.buffer[self.buffer_id]

# -------------------------------------------------------------------------------------------------
# 								PARSER
# -------------------------------------------------------------------------------------------------

	def parse_ka(self):												# analýza souboru

		for c in self.char_gen():
			# dump(c, 'znak')
			if c.isspace():
				pass
			elif c == '(' or c == ')':
				self.brackets_round += 1
			elif c == ',':
				self.commas += 1
				if self.state == 3:
					self.ka_state_start = self.get_start()			# získej počátek
			elif c == '{':
				# dump(self.state, 'stav')
				self.brackets_curly += 1
				if self.state == 0:
					self.ka_states = self.get_states()				# získej stavy
				elif self.state == 1:
					self.ka_alphabet = self.get_alphabet()			# získej abecedu
				elif self.state == 2:
					self.ka_rules = self.get_rules()				# získej pravidla
				elif self.state == 4:
					self.ka_states_end = self.get_states()			# získej koncové stavy
			else:
				error(60, 'neočekávaný znak "' + c + '"')

		if self.brackets_round != 2 or self.brackets_curly != 8 or self.commas != 4:
			error(60, 'špatný počet řídicích znaků')

	def get_states(self):
		self.state += 1
		r = str()
		l = list()
		comma = False

		for c in self.char_gen():
			# dump(c, 'znak')
			if c.isspace():
				pass
			elif c == ',':
				if comma:
					error(60, 'neočekávaná čárka')
				comma = True
				l.append(r)
				r = str()
			elif c == '}':
				if comma:
					error(60, 'neočekávaná čárka')
				self.brackets_curly += 1
				if r != '':
					l.append(r)
				l = list(set(l))
				l.sort()
				dump(l, 'množina stavů')	# dump
				return l
			else:
				comma = False
				r += c


	def get_alphabet(self):
		self.state += 1
		r = str()
		l = list()
		comma = False
		apostrophe = False
		whitespace = True

		for c in self.char_gen():
			# dump(c, 'znak')
			if c.isspace() and whitespace:
				pass
			elif c == '\'':
				comma = False
				apostrophe = not apostrophe
				whitespace = not whitespace
				r += c
			elif c == ',':
				if apostrophe:
					comma = False
					r += c
				else:
					if comma:
						error(60, 'neočekávaná čárka')
					comma = True
					l.append(r)
					r = str()
			elif c == '}':
				if apostrophe:
					comma = False
					r += c
				else:
					if comma:
						error(60, 'neočekávaná čárka')
					self.brackets_curly += 1
					l.append(r)
					l = list(set(l))
					l.sort()
					dump(l, 'vstupní abeceda')	# dump
					return l
			else:
				comma = False
				r += c

	def get_rules(self):
		self.state += 1
		l = list()
		p = ['', '', '']
		state = 0
		apostrophe = False
		whitespace = True

		for c in self.char_gen():
			# dump(c, 'znak')

			if c.isspace() and whitespace:
				pass
			elif state == 0:			# startovní stav
				if c == '\'':
					apostrophe = not apostrophe
					whitespace = not whitespace
					state = 1
					p[1] += c
				else:
					p[0] += c
			elif state == 1:			# znak na pásce
				if c == '\'':
					apostrophe = not apostrophe
					whitespace = not whitespace
					p[1] += c
				elif c == '-':
					if apostrophe:
						p[1] +=c
					else:
						state = 2
						whitespace = False
				else:
					p[1] += c
			elif state == 2:			# šipka
				if c == '>':
					whitespace = True
					state = 3
				else:
					error(60, 'šipečka u pravidel je špatná')
			elif state == 3:			# cílový stav
				if c == ',':
					l.append(p)
					p = ['', '', '']
					state = 0
					apostrophe = False
					whitespace = True
				elif c == '}':
					l.append(p)
					self.brackets_curly += 1
					dump(l, 'množina pravidel')	# dump
					return sorted(l, key=itemgetter(0,1,2))
				else:
					p[2] += c

	def get_start(self):
		self.state += 1
		r = str()
		for c in self.char_gen():
			if c.isspace():
				pass
			elif c == ',':
				self.commas += 1
				dump(r, 'počáteční stav')	# dump
				return r		# konec
			else:
				r += c

# -------------------------------------------------------------------------------------------------
# 								CHECK
# -------------------------------------------------------------------------------------------------

	def check_ka(self):
		self.check_states()
		self.check_alphabet()
		self.check_rules()

	def check_states(self):
		if not self.ka_states:
			error(60, 'prázdná množina stavů')
		for s in self.ka_states:
			if s[0] == '_' or s[-1] == '_' or s[0].isdigit():
				error(60, 'stav "' + s + '" má neplatný název')

		if self.ka_state_start not in self.ka_states:
			error(61, 'počáteční stav není v množině stavů')

		if not self.ka_states_end:
			error(62, 'prázdná množina koncových stavů')
		for s in self.ka_states_end:
			if s not in self.ka_states:
				error(61, 'koncový stav "' + s + '" není v množině v všech stavů')

	def check_alphabet(self):
		if not self.ka_alphabet:
			error(61, 'prázdná abeceda')
		for c in self.ka_alphabet:
			if c == '\'\'':											# ''
				error(60, 'epsilon v abecedě')
			elif len(c) > 3 and c != '\'\'\'\'':					# ''''
				error(60, 'příliš mnoho znaků v "' + c + '"')
			elif c == '\'\'\'':										# '''
				error(60, 'špatně zapsaný apostrof')

	def check_rules(self):
		if not self.ka_rules:
			error(60, 'prázdná množina pravidel')
		for r in self.ka_rules:
			if r[0] not in self.ka_states or r[1] not in self.ka_alphabet or r[2] not in self.ka_states:
				error(61, 'Pravidlo "' + r[0] + ' ' + r[1] + ' -> ' + r[2] + '" je neplatné')

		for s in self.ka_states:									# duplicitní pravidla
			for a in self.ka_alphabet:
				i = 0
				for r in self.ka_rules:
					if r[0] == s and r[1] == a:
						i += 1
				if i > 1:
					error(62, 'duplicitní pravidla pro stav "' + s + '" a symbol ' + a)

# -------------------------------------------------------------------------------------------------
# 								FIND
# -------------------------------------------------------------------------------------------------

	def find_unreachable(self):
		temp = list()
		temp.append(self.ka_state_start)
		l = copy.deepcopy(temp)
		while(True):
			for r in self.ka_rules:
				for s in temp:
					for z in self.ka_alphabet:
						if r[0] == s and r[1] == z:
							l.append(r[2])
			l = list(set(l))
			l.sort()
			if l == temp:
				break
			else:
				temp = copy.deepcopy(l)

		if l != self.ka_states:
			error(62, 'nalezen nedostupný stav')

	def find_non_finishing(self):
		l = list()
		for i in self.ka_states:
			states = list()
			for j in self.ka_alphabet:
				for r in self.ka_rules:
					if r[0] == i and r[1] == j:
						states.append(r[2])
			states = list(set(states))
			if len(states) == 1 and i in states:
				l.append(states[0])
		return l

# -------------------------------------------------------------------------------------------------
# 								MINIMIZE
# -------------------------------------------------------------------------------------------------

	def minimize(self):
		spaces(5)
		states_other = list(set(self.ka_states) - set(self.ka_states_end))

		groups = list()
		groups.append(self.ka_states_end)
		groups.append(states_other)

		temp_states = list()
		temp_groups = list(groups)

		division_counter = 0
		while(True):
			other_states = groups[-1]
			division_flag = False
			for symbol in self.ka_alphabet:
				dump(symbol, 'symbol', 31)
				for group in groups:
					temp_rules = self.group_rules(group)
					for state in group:
						temp_states.append(self.get_end_state_of_rule(state, symbol))
					dump(temp_states, 'temp_states')
					dump(groups, 'groups')
					if not self.same_group(temp_states, groups):
						####################################
						division_counter = 0
						group_01 = []
						group_02 = []
						for m in temp_states:
							key = self.get_key(temp_rules, symbol, m)
							temp_rules = [r for r in temp_rules if r[0] != key]
							if self.is_state_member(m, other_states):
								group_01.append(key)
							else:
								group_02.append(key)

						temp_groups.remove(other_states)
						if len(group_01) > len(group_02):
							temp_groups.append(group_02)
							temp_groups.append(group_01)
						else:
							temp_groups.append(group_01)
							temp_groups.append(group_02)
						division_flag = True
						####################################
					temp_states = list()

				if division_flag:
					break

			if temp_groups == groups:
				division_counter += 1
			if division_counter == 2:
				break
			groups = list(temp_groups)

			spaces()

		groups.sort()
		for g in groups:
			g.sort()
		spaces()
		dump(groups, 'FINAL groups')


		## minimalizovaný automat
		min = Machine()

		# stavy
		min.ka_states = self.min_states(groups)

		# abeceda
		min.ka_alphabet = self.ka_alphabet

		# pravidla
		min.ka_rules = self.new_rules(groups, min.ka_states)

		# počátek
		min.ka_state_start = self.ka_state_start
		if min.ka_state_start not in min.ka_states:
			min.ka_state_start = self.find_merged_state(groups, min.ka_state_start)

		# konce
		end = self.ka_states_end[0]
		if end not in min.ka_states:
			end = self.find_merged_state(groups, end)
		min.ka_states_end.append(end)
		spaces(5)
		return min

# -------------------------------------------------------------------------------------------------
# 								MINIMIZE (FCE-BEFORE)
# -------------------------------------------------------------------------------------------------

	def is_state_member(self, member, group):						# je stav součástí skupiny?
		if member in group:
			return True
		else:
			return False

	def same_group(self, members, groups):							# kontrola skupin
		for g in groups:
			membership = True
			for m in members:
				if not self.is_state_member(m, g):
					membership = False
					break
			else:
				dump(membership, 'membership')
				return membership

	def get_key(self, rules, symbol, state):						# vrátí počateční stav pravidla z množiny určitých pravidel
		for r in rules:
			if r[1] == symbol and r[2] == state:
				return r[0]

	def get_end_state_of_rule(self, state, symbol):					# vrátí konečný stav pro pravidlo
		for r in self.ka_rules:
			if r[0] == state and r[1] == symbol:
				return r[2]

	def group_rules(self, group):									# vrátí pravidla pro danou skupinu stavů
		rules = list()
		for m in group:
			for r in self.ka_rules:
				if r[0] == m:
					rules.append(r)
		return rules

# -------------------------------------------------------------------------------------------------
# 								MINIMIZE (FCE-AFTER)
# -------------------------------------------------------------------------------------------------

	def min_states(self, groups):									# nové stavy po minimalizaci
		states = list()
		for g in groups:
			if len(g) == 1:
				states.append(g[0])
			else:
				states.append("_".join(g))
		states.sort()
		return states

	def find_merged_state(self, groups, state):						# vrátí spojený stav pro původní stav
		for g in groups:
			if state in g:
				return "_".join(g)

	def find_merged(self, groups, state):							# vrátí skupinu stavů, které byli spojeny
		for g in groups:
			if state == '_'.join(g):
				return g[0]

	def new_rules(self, groups, states):							# vytvoří nová pravidla po minimalizaci
		rules = list()
		for state in states:
			for symbol in self.ka_alphabet:
				if state not in self.ka_rules[0]:
					temp_state = self.find_merged(groups, state)
					next_state = self.get_end_state_of_rule(temp_state, symbol)
					if next_state not in states:
						next_state = self.find_merged_state(groups, next_state)
				else:
					next_state = self.get_end_state_of_rule(state, symbol)
					if next_state not in states:
						next_state = self.find_merged_state(groups, next_state)
				rules.append([state, symbol, next_state])
		dump(rules, 'FINAL rules')
		return sorted(rules, key=itemgetter(0,1,2))


# -------------------------------------------------------------------------------------------------
# 								FORMAT
# -------------------------------------------------------------------------------------------------

	def format(self):
		r = str()
		r += '(\n'

		# množina stavů
		r += '{'
		if self.ka_states:
			for i in self.ka_states:
				r += i + ', '
			r = r[:-2]
		r += '},\n'

		#vstupní abeceda
		r += '{'
		if self.ka_alphabet:
			for i in self.ka_alphabet:
				r += i + ', '
			r = r[:-2]
		r += '},\n'

		#množina pravidel
		r += '{\n'
		if self.ka_rules:
			for i in self.ka_rules:
				r += i[0] + ' ' + i[1] + ' -> ' + i[2] + ',\n'
			r = r[:-2]
		r += '\n},\n'

		# počáteční stav
		r += self.ka_state_start
		r += ',\n'

		# množina koncových stavů
		r += '{'
		if self.ka_states_end:
			for i in self.ka_states_end:
				r += i + ', '
			r = r[:-2]
		r += '}\n'

		r += ')\n'
		return r

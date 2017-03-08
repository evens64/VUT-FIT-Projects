class Equation
  def self.solve_quadratic(a, b, c)
		if a == 0	#pokud 'a' je nula
			if b == 0 #a 'b' je taky nula
				return nil	#tak vrať 'nil'
			else
				x = -c.to_f / b
				return [x]	#jinak vrať 'x' (b*x + c = 0 -> x = -c / b)
			end
		else	#pokud 'a' není nula
			d = b**2 - 4*a*c	#vypočitej diskriminant
			if d > 0	#pokud je diskriminant kladný
				x1 = (-b + Math.sqrt(d)).to_f / (2 * a)
				x2 = (-b - Math.sqrt(d)).to_f / (2 * a)
				return [x1, x2]	#vrať dva možné kořeny 'x'
			elsif d == 0	#pokud je diskriminant nulový
				x = -b.to_f / (2 * a)
				return [x]	#vrať jen jeden kořen
			else
				return nil	#jinak 'nil'
			end
		end
  end
end
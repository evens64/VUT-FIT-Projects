require_relative 'fib-sequence-slow'

@F = FibonacciSequence.new

@F.[](100)

print "#{@F.current}\n"
require 'test/unit'
require_relative 'fib-sequence-slow'

class FibonacciSequenceTest < Test::Unit::TestCase
  def setup
		@F = FibonacciSequence.new
  end

	def test_reset
		assert_equal( @F.reset, true )
  end

	def test_current
		assert_equal( @F.current, nil )
  end

	def test_current_idx
		assert_equal( @F.current_idx, nil )
  end

	def test_next
		assert_equal( @F.current, nil )
		assert_equal( @F.current_idx, nil )

		assert_equal( @F.next, 0 )
		assert_equal( @F.current, 0 )
		assert_equal( @F.current_idx, 0 )

		assert_equal( @F.next, 1 )
		assert_equal( @F.current, 1 )
		assert_equal( @F.current_idx, 1 )

		assert_equal( @F.next, 1 )
		assert_equal( @F.current, 1 )
		assert_equal( @F.current_idx, 2 )

		assert_equal( @F.next, 2 )
		assert_equal( @F.current, 2 )
		assert_equal( @F.current_idx, 3 )

		assert_equal( @F.next, 3 )
		assert_equal( @F.current, 3 )
		assert_equal( @F.current_idx, 4 )

		assert_equal( @F.next, 5 )
		assert_equal( @F.current, 5 )
		assert_equal( @F.current_idx, 5 )

		assert_equal( @F.next, 8 )
		assert_equal( @F.current, 8 )
		assert_equal( @F.current_idx, 6 )

		assert_equal( @F.next, 13 )
		assert_equal( @F.current, 13 )
		assert_equal( @F.current_idx, 7 )

		assert_equal( @F.next, 21 )
		assert_equal( @F.current, 21 )
		assert_equal( @F.current_idx, 8 )

		assert_equal( @F.next, 34 )
		assert_equal( @F.current, 34 )
		assert_equal( @F.current_idx, 9 )

		assert_equal( @F.next, 55 )
		assert_equal( @F.current, 55 )
		assert_equal( @F.current_idx, 10 )
  end

	def test_brackets
		assert_equal( @F.[](-1), nil )
		assert_equal( @F.current, nil )
		assert_equal( @F.current_idx, nil )

		assert_equal( @F.[](0), 0 )
		assert_equal( @F.current, 0 )
		assert_equal( @F.current_idx, 0 )

		assert_equal( @F.[](1), 1 )
		assert_equal( @F.current, 1 )
		assert_equal( @F.current_idx, 1 )

		assert_equal( @F.[](2), 1 )
		assert_equal( @F.current, 1 )
		assert_equal( @F.current_idx, 2 )

		assert_equal( @F.[](3), 2 )
		assert_equal( @F.current, 2 )
		assert_equal( @F.current_idx, 3 )

		assert_equal( @F.[](4), 3 )
		assert_equal( @F.current, 3 )
		assert_equal( @F.current_idx, 4 )

		assert_equal( @F.[](5), 5 )
		assert_equal( @F.current, 5 )
		assert_equal( @F.current_idx, 5 )

		assert_equal( @F.[](6), 8 )
		assert_equal( @F.current, 8 )
		assert_equal( @F.current_idx, 6 )

		assert_equal( @F.[](7), 13 )
		assert_equal( @F.current, 13 )
		assert_equal( @F.current_idx, 7 )

		assert_equal( @F.[](8), 21 )
		assert_equal( @F.current, 21 )
		assert_equal( @F.current_idx, 8 )

		assert_equal( @F.[](9), 34 )
		assert_equal( @F.current, 34 )
		assert_equal( @F.current_idx, 9 )

		assert_equal( @F.[](10), 55 )
		assert_equal( @F.current, 55 )
		assert_equal( @F.current_idx, 10 )
	end

	def test_random
		assert_equal( @F.[](9), 34 )
		assert_equal( @F.current, 34 )
		assert_equal( @F.current_idx, 9 )

		assert_equal( @F.[](6), 8 )
		assert_equal( @F.current, 34 )
		assert_equal( @F.current_idx, 9 )

		assert_equal( @F.next, 55 )
		assert_equal( @F.current, 55 )
		assert_equal( @F.current_idx, 10 )
	end

end
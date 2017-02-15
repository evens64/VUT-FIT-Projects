class Main
{
	static void run()
	{
		int a;

		a = 7;
		int vysl;
		if (a < 8) { // nacitani zaporneho cisla nemusite podporovat
			a = a + 1 * 2;
		}
		else {
			vysl = 1;
			while (a > 0) {
			vysl = vysl * a;
			a = a - 1;
			}
		}
	}
}

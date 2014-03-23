import java.util.*;

public class LystBackTrack2 {
	public static final int[] UP = {4,5,8,9,11,12,14};
	public static final int[] DOWN = {2,6,7,9,10,11,14};
	public static final int[] RIGHT = {1,5,6,9,10,12,13};
	public static final int[] LEFT = {3,7,8,10,11,12,13};
	private static long count = 0;
	private static ArrayList<Integer> PIECES = new ArrayList<Integer>();
	
	public static void main(String[] args) {
		int[] startUpTemp = {1,1,1,1,2,2,2,2,3,3,3,3,3,4,5,5,5,5,5,6,6,7,7,8,8,8,9,10,10,10,10,10,11,12,12,12,12,12,13,13,13,13,13,13,13,13,14,14};
		for (int i=0; i<startUpTemp.length; i++) {
			PIECES.add(startUpTemp[i]);
		}
		ArrayList<Integer> l = new ArrayList<Integer>();
		l.add(PIECES.remove(0));
		procedure(l);
	}
	
	public static boolean procedure(ArrayList<Integer> c) {
		count++;
		if (count%1000000==0) {
			System.out.println(c);
			count=0;
		}
		if (reject(c)) {
			return(true);
		} else if (accept(c)) {
			output(c);
		}
		ArrayList<Integer> s = first(c);
		while (s!=null) {
			procedure(s);
			s = next(s);
		}
		return(false);
	}
	
	public static boolean reject(ArrayList<Integer> l) {
		for (int a=0; a<l.size(); a++) {
			int i = a/16;
			int j = a%16;
			if (i==0 && contains(UP,l.get(a))) {
				return(true);
			}
			if (i==2 && contains(DOWN,l.get(a))) {
				return(true);
			}
			if (j==0 && contains(LEFT,l.get(a))) {
				return(true);
			}
			if (j==15 && contains(RIGHT,l.get(a))) {
				return(true);
			}
			if (l.size()>a+1 && j<16) {
				if ((contains(RIGHT,l.get(a)) && !contains(LEFT,l.get(a+1))) || (!contains(RIGHT,l.get(a)) && contains(LEFT,l.get(a+1)))) {
					return(true);
				}
			}
			if (l.size()>a+16 && i<3) {
				if ((contains(DOWN,l.get(a)) && !contains(UP,l.get(a+16))) || (!contains(DOWN,l.get(a)) && contains(UP,l.get(a+16)))) {
					return(true);
				}
			}
			if (l.get(a)==1 && l.size()>a+1) {
				if (3 == l.get(a+1)) {
					return(true);
				}
			}
			if (l.get(a)==2 && l.size()>a+16) {
				if (4 == l.get(a+16)) {
					return(true);
				}
			}
			
		}
		return(false);
	}
	
	public static boolean accept(ArrayList<Integer> l) {
		if (l.size() == 48) {
			return(true);
		}
		return(false);
	}
	
	public static ArrayList<Integer> next(ArrayList<Integer> array) {
		int finalValue = array.get(array.size()-1);
		PIECES.add(array.remove(array.size()-1));
		Collections.sort(PIECES);
		if (finalValue != PIECES.get(PIECES.size()-1)) {
			array.add(PIECES.remove(PIECES.lastIndexOf(finalValue)+1));
			return(array);
		} else {
			return(null);
		}
	}
	
	public static ArrayList<Integer> first(ArrayList<Integer> array) {
		array.add(PIECES.remove(0));
		return(array);
	}
	
	public static void output(ArrayList<Integer> s) {
		for (int i=0; i<3; i++) {
			for (int j=0; j<16; j++) {
				System.out.print(s.get(i*16+j));
			}
			System.out.println();
		}
	}
	
	public static boolean contains(int[] array, int value) {
		for (int a: array) {
			if (a==value) {
				return(true);
			}
		}
		return(false);
	}
}

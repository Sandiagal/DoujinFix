import java.util.ArrayList;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class BasicTool {

	/**
	 * 一维数组求和，返回数值。
	 * 
	 * @param Set
	 *            所求和的一维数组。
	 * @return 总和。
	 */
	public static long sum(long[] Set) {
		long sum = 0;
		for (int i = 0; i < Set.length; i++) {
			sum = sum + Set[i];
		}
		return sum;
	}

	/**
	 * 二维数组求和，返回一维数组。
	 * 
	 * @param Set
	 *            所求和的一维数组。
	 * @return 总和数组。
	 */
	public static long[] sum(long[][] Set) {
		long[] sum = new long[Set.length];
		for (int i = 0; i < Set.length; i++) {
			for (int j = 0; j < Set[i].length; j++) {
				sum[i] = sum[i] + Set[i][j];
			}
		}
		return sum;
	}

	/**
	 * 查找一维数组最后一个非零数的位置，即有效长度。
	 * 
	 * @param Set
	 *            要查找的数组。
	 * @return 有效长度。若数组全为0则返回-1。
	 */
	public static int effectiveLength(int[] Set) {
		for (int i = Set.length - 1; i > -1; i--) {
			if (Set[i] != 0) {
				return i + 1;
			}
		}
		return -1;
	}

	/**
	 * 查找一维数组最后一个非零数的位置，即有效长度。
	 * 
	 * @param Set
	 *            要查找的数组。
	 * @return 有效长度。若数组全为0则返回-1。
	 */
	public static int effectiveLength(long[] Set) {
		for (int i = Set.length - 1; i > -1; i--) {
			if (Set[i] != 0) {
				return i + 1;
			}
		}
		return -1;
	}

	/**
	 * 查找一个一维数组最后一个非零量
	 * 
	 * @param intSet
	 *            被查找的二维数组
	 * @return 查找结果
	 */
	public static int lastNonzero(int[] intSet) {
		for (int i = intSet.length - 1; i > -1; i--) {
			if (intSet[i] != 0) {
				return intSet[i];
			}
		}
		return -1;
	}

	/**
	 * 打印出一维字符串组。
	 * 
	 * @param Set
	 *            所打印的一维字符串组。
	 */
	public static void show(String[] Set) {
		for (int i = 0; Set[i] != null; i++) {
			if (Set[i].length() != 0) {
				System.out.println(i + ":" + Set[i]);
			}
		}
	}

	/**
	 * 打印出二维字符串组。
	 * 
	 * @param Set
	 *            所打印的一维字符串组。
	 */
	public static void show(String[][] Set) {
		for (int i = 0; Set[i][0] != null; i++) {
			for (int j = 0; Set[i][j] != null; j++) {
				if (Set[i][j].length() != 0) {
					System.out.println(i + ". " + j + ": " + Set[i][j]);
				}
			}
			System.out.println("");
		}
	}

	/**
	 * 打印出一维数组。
	 * 
	 * @param Set
	 *            所打印的一维数组。
	 */
	public static void show(int[] Set) {
		for (int i = 0; i < effectiveLength(Set); i++) {
			System.out.print(Set[i] + " ");
		}
		System.out.println("");
	}

	/**
	 * 打印出二维数组。
	 * 
	 * @param Set
	 *            所打印的一维数组。
	 */
	public static void show(long[][] Set) {
		for (int i = 0; i < effectiveLength(sum(Set)); i++) {
			System.out.print(i + ":");
			for (int j = 0; j < effectiveLength(Set[i]); j++) {
				System.out.print(Set[i][j] + " ");
			}
			System.out.println("");
		}
		System.out.println("");
	}

	/**
	 * 求一维数组中最大值。
	 * 
	 * @param Set
	 *            所打印的一维数组。
	 * @return 最大值。
	 */
	public static int max(int[] Set) {
		int max = Set[0];
		for (int i = 1; i < Set.length; i++) {
			if (max < Set[i]) {
				max = Set[i];
			}
		}
		return max;
	}

	/**
	 * 计算两个数值的异或值。把非零值当作1.
	 * 
	 * @param value1
	 *            数值1
	 * @param value2
	 *            数值2
	 * @return 异或值
	 */
	public static int xor(int value1, int value2) {
		if (value1 * value2 == 0 && value1 + value2 != 0) {
			return 1;
		}
		return 0;
	}

	/**
	 * 查找字符串中所有含指定字符串的所有位置，用一维数组返回位置
	 * 
	 * @param string
	 *            主字符串
	 * @param aimString
	 *            要找字符串
	 * @return 指定字符位置
	 */
	public static int[] findAll(String string, String aimString) {
		int[] rslt = new int[string.length()];
		int i = -2, j = 0;
		while ((i = string.indexOf(aimString, i + 1)) != -1) {
			rslt[j] = i;
			j++;
		}
		return rslt;
	}

	/**
	 * 查找一个范围内是否包含一个数组中的数字，存在则返回该数字在数组中的位置，否则返回-1
	 * 
	 * @param lowEdge
	 *            查找下届
	 * @param upEdge
	 *            查找上届
	 * @param eare
	 *            查找的数
	 * @return 该数字在数组中位置
	 */
	public static int eareFind(int lowEdge, int upEdge, int[] eare) {
		for (int i = 0; i < eare.length; i++) {
			if (lowEdge < eare[i] && eare[i] < upEdge) {
				return i;
			}
		}
		return -1;
	}

	/**
	 * 在a字符串的t位置添加b字符串
	 * 
	 * @param a
	 *            被添加字符串
	 * @param b
	 *            添加字符串
	 * @param t
	 *            添加位置
	 * @return 返回结果
	 */
	public static String stringInsert(String a, String b, int t) {
		return a.substring(0, t) + b + a.substring(t, a.length());
	}

	public static void main(String[] args) {

	}

}

import java.io.IOException;

public class Graphics {

	/**
	 * ʹ��Runtime�����exec�����������ⲿexe�ļ���
	 */
	public static void runExe(String args) {
		Runtime rt = Runtime.getRuntime();
		try {
			rt.exec(args);
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

}

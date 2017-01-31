import java.io.IOException;

public class Graphics {

	/**
	 * 使用Runtime对象的exec方法，调用外部exe文件。
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

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.PrintWriter;

public class FileOperate {
	private static int TXTLength = 1024; // ���ܶ�ȡTXT�ĵ����������
	private static int folderTotal = 1024; // ���ܴ��������ļ�����
	private static int fileTotalPerFolder = 1024; // �ļ����ڿ��ܴ��������ļ���

	/**
	 * �½�Ŀ¼
	 * 
	 * @param folderPath
	 *            String �� c:/fqf
	 * @return boolean
	 */
	public static void newFolder(String folderPath) {
		try {
			String filePath = folderPath;
			filePath = filePath.toString();
			java.io.File myFilePath = new java.io.File(filePath);
			if (!myFilePath.exists()) {
				myFilePath.mkdir();
			}
		} catch (Exception e) {
			System.out.println("�½�Ŀ¼��������");
			e.printStackTrace();
		}
	}

	/**
	 * �½��ļ�
	 * 
	 * @param filePathAndName
	 *            String �ļ�·�������� ��c:/fqf.txt
	 * @param fileContent
	 *            String �ļ�����
	 * @return boolean
	 */
	public static void newFile(String filePathAndName, String fileContent) {
		try {
			String filePath = filePathAndName;
			filePath = filePath.toString();
			File myFilePath = new File(filePath);
			if (!myFilePath.exists()) {
				myFilePath.createNewFile();
			}
			FileWriter resultFile = new FileWriter(myFilePath);
			PrintWriter myFile = new PrintWriter(resultFile);
			String strContent = fileContent;
			myFile.println(strContent);
			resultFile.close();
		} catch (Exception e) {
			System.out.println("�½�Ŀ¼��������");
			e.printStackTrace();
		}
	}

	/**
	 * ɾ���ļ�
	 * 
	 * @param filePathAndName
	 *            String �ļ�·�������� ��c:/fqf.txt
	 * @param fileContent
	 *            String
	 * @return boolean
	 */
	public static void delFile(String filePathAndName) {
		try {
			String filePath = filePathAndName;
			filePath = filePath.toString();
			java.io.File myDelFile = new java.io.File(filePath);
			myDelFile.delete();
		} catch (Exception e) {
			System.out.println("ɾ���ļ���������");
			e.printStackTrace();
		}
	}

	/**
	 * ɾ���ļ���
	 * 
	 * @param filePathAndName
	 *            String �ļ���·�������� ��c:/fqf
	 * @param fileContent
	 *            String
	 * @return boolean
	 */
	public static void deleteFolder(String folderPath) {
		try {
			deleteAllFile(folderPath); // ɾ����������������
			String filePath = folderPath;
			filePath = filePath.toString();
			java.io.File myFilePath = new java.io.File(filePath);
			myFilePath.delete(); // ɾ�����ļ���
		} catch (Exception e) {
			System.out.println("ɾ���ļ��в�������");
			e.printStackTrace();
		}
	}

	/**
	 * ɾ���ļ�������������ļ�
	 * 
	 * @param path
	 *            String �ļ���·�� �� c:/fqf
	 */
	public static void deleteAllFile(String path) {
		File file = new File(path);
		if (!file.exists()) {
			return;
		}
		if (!file.isDirectory()) {
			return;
		}
		String[] tempList = file.list();
		File temp = null;
		for (int i = 0; i < tempList.length; i++) {
			if (path.endsWith(File.separator)) {
				temp = new File(path + tempList[i]);
			} else {
				temp = new File(path + File.separator + tempList[i]);
			}
			if (temp.isFile()) {
				temp.delete();
			}
			if (temp.isDirectory()) {
				deleteAllFile(path + "/" + tempList[i]);// ��ɾ���ļ���������ļ�
				deleteFolder(path + "/" + tempList[i]);// ��ɾ�����ļ���
			}
		}
	}

	/**
	 * ���Ƶ����ļ�
	 * 
	 * @param oldPath
	 *            String ԭ�ļ�·�� �磺c:/fqf.txt
	 * @param newPath
	 *            String ���ƺ�·�� �磺f:/fqf.txt
	 * @return boolean
	 */
	public static void copyFile(String oldPath, String newPath) {
		try {
			// int bytesum = 0;
			int byteread = 0;
			File oldfile = new File(oldPath);
			if (oldfile.exists()) { // �ļ�����ʱ
				InputStream inStream = new FileInputStream(oldPath); // ����ԭ�ļ�
				FileOutputStream fs = new FileOutputStream(newPath);
				byte[] buffer = new byte[1444];
				while ((byteread = inStream.read(buffer)) != -1) {
					// bytesum += byteread; // �ֽ��� �ļ���С
					// System.out.println(bytesum);
					fs.write(buffer, 0, byteread);
				}
				inStream.close();
				fs.close();
			}
		} catch (Exception e) {
			System.out.println("���Ƶ����ļ���������");
			e.printStackTrace();
		}
	}

	/**
	 * ���������ļ�������
	 * 
	 * @param oldPath
	 *            String ԭ�ļ�·�� �磺c:/fqf
	 * @param newPath
	 *            String ���ƺ�·�� �磺f:/fqf/ff
	 * @return boolean
	 */
	public static void copyFolder(String oldPath, String newPath) {
		try {
			(new File(newPath)).mkdirs(); // ����ļ��в����� �������ļ���
			File a = new File(oldPath);
			String[] file = a.list();
			File temp = null;
			for (int i = 0; i < file.length; i++) {
				if (oldPath.endsWith(File.separator)) {
					temp = new File(oldPath + file[i]);
				} else {
					temp = new File(oldPath + File.separator + file[i]);
				}
				if (temp.isFile()) {
					FileInputStream input = new FileInputStream(temp);
					FileOutputStream output = new FileOutputStream(newPath + "/" + (temp.getName()).toString());
					byte[] b = new byte[1024 * 5];
					int len;
					while ((len = input.read(b)) != -1) {
						output.write(b, 0, len);
					}
					output.flush();
					output.close();
					input.close();
				}
				if (temp.isDirectory()) {// ��������ļ���
					copyFolder(oldPath + "/" + file[i], newPath + "/" + file[i]);
				}
			}
		} catch (Exception e) {
			System.out.println("���������ļ������ݲ�������");
			e.printStackTrace();
		}
	}

	/**
	 * �ƶ��ļ���ָ��Ŀ¼
	 * 
	 * @param oldPath
	 *            String �磺c:/fqf.txt
	 * @param newPath
	 *            String �磺d:/fqf.txt
	 */
	public static void moveFile(String oldPath, String newPath) {
		copyFile(oldPath, newPath);
		delFile(oldPath);
	}

	/**
	 * �ƶ��ļ���ָ��Ŀ¼
	 * 
	 * @param oldPath
	 *            String �磺c:/fqf.txt
	 * @param newPath
	 *            String �磺d:/fqf.txt
	 */
	public static void moveFolder(String oldPath, String newPath) {
		copyFolder(oldPath, newPath);
		deleteFolder(oldPath);
	}

	/**
	 * �ƶ��ļ���ָ��Ŀ¼
	 * 
	 * @param oldPath
	 *            String �磺c:/fqf.txt
	 * @param newPath
	 *            String �磺d:/fqf.txt
	 */
	public static void moveFolder2(String oldPath, String newPath) {
		File old = new File(oldPath);
		File rname = new File(newPath);
		try {
			old.renameTo(rname);
		} catch (Exception e) {
			System.out.println("���������ļ������ݲ�������");
			e.printStackTrace();
		}
	}

	/**
	 * �����ļ���ָ��Ŀ¼,��ɾ��ָ��Ŀ¼ԭ���ļ�
	 * 
	 * @param oldPath
	 *            String �磺c:/fqf.txt
	 * @param newPath
	 *            String �磺d:/fqf.txt
	 * @param old2new
	 *            String �磺d:/fqf.txt
	 */
	public static void cutFile(String oldPath, String newPath, String old2new) {
		delFile(newPath);
		moveFolder2(oldPath, old2new);
	}

	/**
	 * ��ȡ��Ŀ¼��ָ������TXT�ĵ����ݣ����ַ���������ʽ����
	 * 
	 * @param txtName
	 *            ָ����TXT������
	 * 
	 * @return �����ĵ�����
	 */
	public static String[] readchangeInfo(String txtName) {
		String[] changeInfo = new String[TXTLength];
		try {
			File file = new File("changeInfo.txt");
			if (file.isFile() && file.exists()) {
				InputStreamReader isr = new InputStreamReader(new FileInputStream(file), "unicode");
				BufferedReader br = new BufferedReader(isr);
				String s = null;
				for (int i = 0; (s = br.readLine()) != null; i++) {
					// System.out.println(s);
					changeInfo[i] = s;
				}
				br.close();
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		return changeInfo;
	}

	/**
	 * �����ض��ļ������������ļ������ƣ������ַ���������ʽ����
	 * 
	 * @param pathSource
	 *            ·������
	 * @return �������
	 */
	public static String[] readfolderName(String pathSource) {
		String[] fldrName = new String[folderTotal];
		File file = new File(pathSource);
		File[] array = file.listFiles();
		for (int i = 0, j = 0; i < array.length; i++) {
			if (array[i].isDirectory()) {
				fldrName[j++] = array[i].getName();
			}
		}
		return fldrName;
	}

	/**
	 * ��ȡ�ļ���ʵ������
	 * 
	 * @param string
	 *            �ļ������ַ�������
	 * @return ��ȡ���
	 */
	public static int realStringSetLength(String[] string) {
		int realLength = 0;
		for (int i = 0; string[i] != null; i++) {
			realLength += 1;
		}
		return realLength;
	}

	/**
	 * ����һϵ���ļ������������ļ����ƣ����Զ�ά�ַ���������ʽ����
	 * 
	 * @param folderPath
	 *            ����һϵ���ļ��е�ַ
	 * 
	 * @return �������
	 */
	public static String[][] readFileName(String path, String[] fldrName) {
		String[][] fileName = new String[folderTotal][fileTotalPerFolder];
		int i = 0;
		while (fldrName[i] != null) {
			File file = new File(path + "\\" + fldrName[i]);
			File[] array = file.listFiles();
			for (int j = 0; j < array.length; j++) {
				fileName[i][j] = array[j].getName();
			}
			i++;
		}
		return fileName;
	}

	/**
	 * ��ȡÿ���ļ�����ʵ���ļ�����
	 * 
	 * @param string
	 *            �ļ�����ά�ַ�������
	 * @param length
	 *            �ļ���ʵ������
	 * @return ��ȡ���
	 */
	public static int[] realStringSetLength(String[][] string, int length) {
		int[] realLength = new int[folderTotal];
		for (int i = 0; i < length; i++) {
			for (int j = 0; string[i][j] != null; j++) {
				realLength[i] += 1;
			}
		}
		return realLength;
	}

	/**
	 * ����һϵ���ļ������������ļ���С�����Զ�ά������ʽ����
	 * 
	 * @param path
	 *            �����ļ��е�ַ
	 * 
	 * @param folderPath
	 *            ����һϵ���ļ��е�ַ
	 * 
	 * @return �������
	 */
	public static long[][] readFileSize(String path, String[] fldrName, String[][] fileName) {
		long[][] fileSize = new long[folderTotal][fileTotalPerFolder];
		for (int i = 0; fileName[i][0] != null; i++) {
			for (int j = 0; fileName[i][j] != null; j++) {
				File file = new File(path + "\\" + fldrName[i] + "\\" + fileName[i][j]);
				fileSize[i][j] = file.length();
			}
		}
		return fileSize;
	}

	public static void main(String[] args) {
		String pathSource = "E:\\Temporary\\1Check\\2Check\\(Batch)";
		String pathAim = "E:\\Temporary\\�½��ļ���";

		String[] fldrName = readfolderName(pathSource);
		BasicTool.show(fldrName);

		int folderTotalReal = realStringSetLength(fldrName);
		System.out.println(folderTotalReal);

		String[][] fileNameSource = readFileName(pathSource, fldrName);
		BasicTool.show(fileNameSource);

		long[][] fileSizeSource = readFileSize(pathSource, fldrName, fileNameSource);
		BasicTool.show(fileSizeSource);

	}

}

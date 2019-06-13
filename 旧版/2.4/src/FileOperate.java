import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.PrintWriter;

public class FileOperate {
	private static int TXTLength = 1024; // 可能读取TXT文档的最大行数
	private static int folderTotal = 1024; // 可能处理的最大文件夹数
	private static int fileTotalPerFolder = 1024; // 文件夹内可能处理的最大文件数

	/**
	 * 新建目录
	 * 
	 * @param folderPath
	 *            String 如 c:/fqf
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
			System.out.println("新建目录操作出错");
			e.printStackTrace();
		}
	}

	/**
	 * 新建文件
	 * 
	 * @param filePathAndName
	 *            String 文件路径及名称 如c:/fqf.txt
	 * @param fileContent
	 *            String 文件内容
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
			System.out.println("新建目录操作出错");
			e.printStackTrace();
		}
	}

	/**
	 * 删除文件
	 * 
	 * @param filePathAndName
	 *            String 文件路径及名称 如c:/fqf.txt
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
			System.out.println("删除文件操作出错");
			e.printStackTrace();
		}
	}

	/**
	 * 删除文件夹
	 * 
	 * @param filePathAndName
	 *            String 文件夹路径及名称 如c:/fqf
	 * @param fileContent
	 *            String
	 * @return boolean
	 */
	public static void deleteFolder(String folderPath) {
		try {
			deleteAllFile(folderPath); // 删除完里面所有内容
			String filePath = folderPath;
			filePath = filePath.toString();
			java.io.File myFilePath = new java.io.File(filePath);
			myFilePath.delete(); // 删除空文件夹
		} catch (Exception e) {
			System.out.println("删除文件夹操作出错");
			e.printStackTrace();
		}
	}

	/**
	 * 删除文件夹里面的所有文件
	 * 
	 * @param path
	 *            String 文件夹路径 如 c:/fqf
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
				deleteAllFile(path + "/" + tempList[i]);// 先删除文件夹里面的文件
				deleteFolder(path + "/" + tempList[i]);// 再删除空文件夹
			}
		}
	}

	/**
	 * 复制单个文件
	 * 
	 * @param oldPath
	 *            String 原文件路径 如：c:/fqf.txt
	 * @param newPath
	 *            String 复制后路径 如：f:/fqf.txt
	 * @return boolean
	 */
	public static void copyFile(String oldPath, String newPath) {
		try {
			// int bytesum = 0;
			int byteread = 0;
			File oldfile = new File(oldPath);
			if (oldfile.exists()) { // 文件存在时
				InputStream inStream = new FileInputStream(oldPath); // 读入原文件
				FileOutputStream fs = new FileOutputStream(newPath);
				byte[] buffer = new byte[1444];
				while ((byteread = inStream.read(buffer)) != -1) {
					// bytesum += byteread; // 字节数 文件大小
					// System.out.println(bytesum);
					fs.write(buffer, 0, byteread);
				}
				inStream.close();
				fs.close();
			}
		} catch (Exception e) {
			System.out.println("复制单个文件操作出错");
			e.printStackTrace();
		}
	}

	/**
	 * 复制整个文件夹内容
	 * 
	 * @param oldPath
	 *            String 原文件路径 如：c:/fqf
	 * @param newPath
	 *            String 复制后路径 如：f:/fqf/ff
	 * @return boolean
	 */
	public static void copyFolder(String oldPath, String newPath) {
		try {
			(new File(newPath)).mkdirs(); // 如果文件夹不存在 则建立新文件夹
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
				if (temp.isDirectory()) {// 如果是子文件夹
					copyFolder(oldPath + "/" + file[i], newPath + "/" + file[i]);
				}
			}
		} catch (Exception e) {
			System.out.println("复制整个文件夹内容操作出错");
			e.printStackTrace();
		}
	}

	/**
	 * 移动文件到指定目录
	 * 
	 * @param oldPath
	 *            String 如：c:/fqf.txt
	 * @param newPath
	 *            String 如：d:/fqf.txt
	 */
	public static void moveFile(String oldPath, String newPath) {
		copyFile(oldPath, newPath);
		delFile(oldPath);
	}

	/**
	 * 移动文件到指定目录
	 * 
	 * @param oldPath
	 *            String 如：c:/fqf.txt
	 * @param newPath
	 *            String 如：d:/fqf.txt
	 */
	public static void moveFolder(String oldPath, String newPath) {
		copyFolder(oldPath, newPath);
		deleteFolder(oldPath);
	}

	/**
	 * 移动文件到指定目录
	 * 
	 * @param oldPath
	 *            String 如：c:/fqf.txt
	 * @param newPath
	 *            String 如：d:/fqf.txt
	 */
	public static void moveFolder2(String oldPath, String newPath) {
		File old = new File(oldPath);
		File rname = new File(newPath);
		try {
			old.renameTo(rname);
		} catch (Exception e) {
			System.out.println("复制整个文件夹内容操作出错");
			e.printStackTrace();
		}
	}

	/**
	 * 剪切文件到指定目录,并删除指定目录原有文件
	 * 
	 * @param oldPath
	 *            String 如：c:/fqf.txt
	 * @param newPath
	 *            String 如：d:/fqf.txt
	 * @param old2new
	 *            String 如：d:/fqf.txt
	 */
	public static void cutFile(String oldPath, String newPath, String old2new) {
		delFile(newPath);
		moveFolder2(oldPath, old2new);
	}

	/**
	 * 读取根目录下指定名称TXT文档内容，以字符串数组形式返回
	 * 
	 * @param txtName
	 *            指定的TXT文名称
	 * 
	 * @return 返回文档内容
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
	 * 遍历特定文件夹下所有子文件夹名称，并以字符串数组形式返回
	 * 
	 * @param pathSource
	 *            路径名称
	 * @return 遍历结果
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
	 * 读取文件夹实际数量
	 * 
	 * @param string
	 *            文件夹名字符串数据
	 * @return 读取结果
	 */
	public static int realStringSetLength(String[] string) {
		int realLength = 0;
		for (int i = 0; string[i] != null; i++) {
			realLength += 1;
		}
		return realLength;
	}

	/**
	 * 遍历一系列文件夹下所有子文件名称，并以二维字符串数组形式返回
	 * 
	 * @param folderPath
	 *            所在一系列文件夹地址
	 * 
	 * @return 遍历结果
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
	 * 读取每个文件夹内实际文件数量
	 * 
	 * @param string
	 *            文件名二维字符串数据
	 * @param length
	 *            文件夹实际数量
	 * @return 读取结果
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
	 * 遍历一系列文件夹下所有子文件大小，并以二维数组形式返回
	 * 
	 * @param path
	 *            所在文件夹地址
	 * 
	 * @param folderPath
	 *            所在一系列文件夹地址
	 * 
	 * @return 遍历结果
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
		String pathAim = "E:\\Temporary\\新建文件夹";

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

import java.util.ArrayList;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class PendingFile {
	/**
	 * 单独测试各个函数模式
	 */
	private static int sglMode = 0;
	/**
	 * 是否列出处理时每一步的结果
	 */
	private static int debugMode = 1;
	/**
	 * 是否显示结果
	 */
	private static int ShowRslt = 1;
	/**
	 * 文件夹的有效处理总数
	 */
	private static int fldrEfflAmt;
	/**
	 * 每个文件夹可能内存在图片总数
	 */
	private static int fileMaxAmtPerFolder = 1024;
	/**
	 * 每个文件夹内存在的有效图片总数
	 */
	private static int[] fileEfflAmtPerFolder;
	/**
	 * 体积大小百分比阈值
	 */
	private static int sizeThrshld = 3;
	/**
	 * 现在处理的文件夹编号
	 */
	private static int nowFldr;
	/**
	 * 现在处理的文件编号
	 */
	private static int nowFile;
	/**
	 * 处理完的文件计数
	 */
	private static int[] cmplteAmt = new int[4];
	/**
	 * 文件标准化命名格式 0:"标记 [制作方名 (作者名)] 作品名 (原作名) [汉化组名]" 1:
	 * "标记 (时间) [制作方名 (作者名)] 作品名 (原作名) [汉化组名]" 2:
	 * "标记 [汉化组名] (时间) [制作方名 (作者名)] 作品名 (原作名)"
	 */
	private static int stdFmt;
	/**
	 * 工作标记 0:标准化数据准备开始 1:标准化数据准备完毕 2:标准化工作开始 3:标准化工作中 4:标准化工作结束 5:转移数据准备开始
	 * 6:转移数据准备完毕 7:转移工作开始 8:转移工作中 9:转移工作结束 10:不必进行工作 11:标准化撤销开始
	 */
	private static int workState = 0;
	/**
	 * 出错标记 1:括号检验 2:提取制作方 3:提取作者 4:提取作品名 5:提取原作名 6:提取汉化组 7:移动文件 8:文件数不匹配
	 * 9:标准化初始化失败 10:文件转移初始化失败 11:正则匹配预处理失败 12：正则匹配读取信息失败
	 */
	private static int[] errCode = new int[1];
	/**
	 * 源文件夹总路径
	 */
	private static String srcPath;
	/**
	 * 目标文件夹总路径
	 */
	private static String aimPath;
	/**
	 * 文件夹名称
	 */
	private static String[] fldrName;
	/**
	 * 原作名标准化转换对
	 */
	private static String[] chgInfo;
	/**
	 * 标准化文件夹名称
	 */
	private static String[] stdFldrName;
	/**
	 * 源文件夹内文件名称
	 */
	private static String[][] srcFileName;
	/**
	 * 目标文件夹内文件名称
	 */
	private static String[][] aimFileName;
	/**
	 * 源文件夹内文件大小
	 */
	private static long[][] srcFileSize;
	/**
	 * 目标文件夹内文件大小
	 */
	private static long[][] aimFileSize;
	/**
	 * 源文件夹内文件大小大于目标文件夹内文件大小的数值
	 */
	private static long[][] srcBigerAim;
	/**
	 * 源文件夹内文件大小是否足够大于目标文件夹内文件大小
	 */
	private static long[][] bigEnuff;
	/**
	 * 临时变量
	 */
	private static int temp = 0;

	public PendingFile() {
		workState = 0;
	}

	public void setSrcPath(String srcPath) {
		PendingFile.srcPath = srcPath;
	}

	public void setAimPath(String aimPath) {
		PendingFile.aimPath = aimPath;
	}

	public void setStdFmt(int stdFmt) {
		PendingFile.stdFmt = stdFmt;
	}

	public int getNowFldr() {
		return nowFldr;
	}

	public void setNowFldr(int nowFldr) {
		PendingFile.nowFldr = nowFldr;
	}

	public int getWorkState() {
		return workState;
	}

	public void setWorkState(int workState) {
		PendingFile.workState = workState;
	}

	public static void setAimFileSize(long[][] aimFileSize) {
		PendingFile.aimFileSize = aimFileSize;
	}

	public int getFldrEfflAmt() {
		return fldrEfflAmt;
	}

	public void setSizeThrshld(int sizeThrshld) {
		PendingFile.sizeThrshld = sizeThrshld;
	}

	public int[] getFileEfflAmtPerFolder() {
		return fileEfflAmtPerFolder;
	}

	public int[] getErrCode() {
		return errCode;
	}

	public int getNowFile() {
		return nowFile;
	}

	/**
	 * 系统初始化，收集标准化命名需要的数据。 初始变量包括开始工作标记，当前工作文件夹标记，完成计数，工作状态标记，错误标记。
	 * 数据包括源文件夹名称，文件夹有效数量，标准原作名替换对。
	 * 
	 * @return 如果该初始化失败则返回失败信息。
	 */
	private static String dataCollectionStandardization() {
		try {
			fldrName = FileOperate.readfolderName(srcPath);
			chgInfo = FileOperate.readchangeInfo("chgInfo.txt");
			fldrEfflAmt = FileOperate.realStringSetLength(fldrName);
			stdFldrName = fldrName.clone();
			srcFileName = FileOperate.readFileName(srcPath, fldrName);
			nowFldr = -1;
			cmplteAmt[0] = 0;
			cmplteAmt[1] = 0;
			workState = 1;
			errCode = new int[fldrEfflAmt];
		} catch (Exception e) {
			errCode[0] = 9;
			return "/nヾ(ﾟдﾟ)ﾉ゛ Data collection for standard failed./n(｀・-・)=3 Try to check the Source Folder/n";
		}
		return "";
	}

	/**
	 * 替换掉字符串中的一些字符串，用于预处理字符串
	 * 
	 * @param pretreatmentString
	 *            被替换的字符串
	 * @return 替换结果
	 */
	private static String stringReplacement(String string) {
		string = string.replace("【", "[");
		string = string.replace("】", "]");
		string = string.replace("（", "(");
		string = string.replace("）", ")");
		string = string.replace("～", "~");
		string = string.replace("[DL版]", "");
		string = string.replace("[Digital]", "");
		string = string.replace("[無修正]", "");
		string = string.replace("(Various)", "(よろず)");
		string = string.replace("(オリジナル)", "(Original)");
		string = string.replace("] ", "]");
		string = string.replace(" [", "[");
		string = string.replace(" (", "(");
		string = string.replace(") ", ")");
		while (string.indexOf("  ") != -1) {
			string = string.replace("  ", " ");
		}
		return string;
	}

	/**
	 * 替换掉字符串中的一些字符串，用于得到标准化名称后
	 * 
	 * @param pretreatmentString
	 *            被替换的字符串
	 * @return 替换结果
	 */
	private static String stringReplacement2(String string) {
		string = string.replace("[ ()]", "");
		string = string.replace("[]", "");
		string = string.replace("()", "");
		while (string.indexOf("  ") != -1) {
			string = string.replace("  ", " ");
		}
		return string;
	}

	/**
	 * 首先判断字符串是否含有"[]"或"()"，若没有，则该文件除了名称外没有更多信息，可以直接输出。
	 * 在有"[]"或"()"的情况下，若只存在一个"[]"，则认为该"[]"内是原制作方信息。
	 * 在此情况下，在字符串末尾添加空"[]"，若"[]"内是否存在"()"，则表明存在作者信息，无则在"[]"内添加空"()"，若"[]"前存在"()"
	 * ，则表明存在时间信息，无则在"[]"前添加空"()"。 在存在2个及以上的"[]"的情况下，则认为包含原制作方信息和汉化组信息。
	 * 若有一个"[]"在字符串的末尾，则认为该"[]"内是汉化组信息，第一个"[]"内是原制作方信息。
	 * 若没有"[]"在字符串的末尾，则认为第一个"[]"内是汉化组信息，第二个"[]"内是原制作方信息。
	 * 在不包含"()"情况下，若字符串以"[]"结尾，则为格式2，否则为格式1。在格式2情况下，在第一个"[]"内后方和前方添加"()"。
	 * 在格式1情况下，在第二个"[]"内后方和前方添加"()"。若只含一个"()" 检测包含原制作方信息的"[]"内是否存在"()"，
	 * 无则在"[]"内添加空"()"。 若字符串以"[]"结尾，而"[]"前没有紧跟"()"，则认为缺乏原作名信息，在"[]"前添加空"()"。
	 * 若字符串没有以"[]"或"()"结尾，则一定缺乏原作名信息，在字符串末尾前添加空"()"。 输出文件形式代码对应以下结构：0="~~~"、
	 * 1="[~~~]()[~~~(~~~)]~~~(~~~)"、2="()[~~~(~~~)]~~~(~~~)[~~~]"。
	 * 
	 * @param string
	 *            预处理的字符串
	 * @param BF
	 *            中前括号位置
	 * @param BB
	 *            中后括号位置
	 * @param SF
	 *            小前括号位置
	 * @param SB
	 *            小后括号位置
	 * @return 工作结果，第一个是字符串处理结果，第二个是处理格式
	 */
	private static String[] Pretreatment(String string, int[] BF, int[] BB, int[] SF, int[] SB) {
		String[] treating = new String[2];
		if (BB[0] == 0 && SB[0] == 0) {// 没有"[]"和"()"
			treating[0] = string;
			treating[1] = "0";
			return treating;
		} else {// 存在"[]"或"()"
			int pretreatment = 0;// 是否再次括号寻找
			if (BB[0] != 0 && BB[1] == 0)// 只有一个"[]"且不包含"()"
			{
				string = BasicTool.stringInsert(string, "[]", string.length());
				treating[1] = "2";
				pretreatment = 1;
				if (BasicTool.eareFind(BF[0], BB[0], SB) == -1) {
					string = BasicTool.stringInsert(string, "()", BB[0]);
				}
			} else {
				if (BB[1] != 0 && BasicTool.eareFind(BF[0], BB[0], SB) == -1
						&& BasicTool.eareFind(BF[1], BB[1], SB) == -1)// 有2个"[]"且不包含"()"
				{
					if (BB[1] == string.length() - 1)// "[]"在最后，即汉化组在后
					{
						string = BasicTool.stringInsert(string, "()", BB[0]);// 在原制作方信息内补充"()"
						treating[1] = "2";
						pretreatment = 1;
					} else {
						string = BasicTool.stringInsert(string, "()", BB[1]);// 在原制作方信息内补充"()"
						treating[1] = "1";
						pretreatment = 1;
					}
				} else { // 有2个"[]"且包含"()"
					if (BasicTool.eareFind(BF[0], BB[0], SB) != -1) {// 第一个"[]"内包含"()"
						treating[1] = "2";
					} else {
						treating[1] = "1";
					}
				}

			}
			if (pretreatment == 1) {
				BF = BasicTool.findAll(string, "[");
				BB = BasicTool.findAll(string, "]");
				SF = BasicTool.findAll(string, "(");
				SB = BasicTool.findAll(string, ")");
				pretreatment = 0;
			}
			if (BasicTool.lastNonzero(BB) == string.length() - 1)// 汉化组在最后
			{
				if (BasicTool.lastNonzero(SB) < BasicTool.lastNonzero(BF) - 2)// 汉化组前没有原作名
				{
					string = BasicTool.stringInsert(string, "()", BasicTool.lastNonzero(BF));
					pretreatment = 1;
				}
			}
			if (string.indexOf(')', string.length() - 1) == -1 && string.indexOf(']', string.length() - 1) == -1)// 文件名没有以)或]结尾
			{
				string = BasicTool.stringInsert(string, "()", string.length());
				pretreatment = 1;
			}
			if (pretreatment == 1) {
				BF = BasicTool.findAll(string, "[");
				BB = BasicTool.findAll(string, "]");
				SF = BasicTool.findAll(string, "(");
				SB = BasicTool.findAll(string, ")");
				pretreatment = 0;
			}
		}
		treating[0] = string;
		return treating;
	}

	/**
	 * 括号数检验。从数组的第二项起，检验每个前括号是否有与之对应的后括号。 检验是否存在在中文字符中存在括号的情况，若有则返回1
	 * 
	 * @param BF
	 *            中前括号位置
	 * @param BB
	 *            中后括号位置
	 * @param SF
	 *            小前括号位置
	 * @param SB
	 *            小后括号位置
	 * @return 检验结果。若数量对应，则返回0；否则，返回1.
	 */
	private static int bracketTesting(int[] BF, int[] BB, int[] SF, int[] SB) {
		int[] brktMaxAmt = { BF.length, BB.length, SF.length, SB.length };
		for (int i = 1; i < BasicTool.max(brktMaxAmt); i++) {
			if (BasicTool.xor(BF[i], BB[i]) == 1 || BasicTool.xor(SF[i], SB[i]) == 1) {
				return 1;
			}
		}
		return 0;
	}

	/**
	 * 读取原制作方信息，位于带"()"的"[]"中"()"前
	 * 
	 * @param string
	 *            被读取的字符串
	 * @param treatFmt
	 *            处理格式:0="~~~"、
	 *            1="[~~~][~~~(~~~)]~~~(~~~)"、2="[~~~(~~~)]~~~(~~~)[~~~]"
	 * @param BF
	 *            中前括号位置
	 * @param BB
	 *            中后括号位置
	 * @param SF
	 *            小前括号位置
	 * @return
	 */
	private static String readProducers(String string, int treatFmt, int[] BF, int[] BB, int[] SF) {
		// 0="~~~"、 1="[~~~][~~~(~~~)]~~~(~~~)"、2="[~~~(~~~)]~~~(~~~)[~~~]"。
		if (treatFmt == 0) {
			return "";
		} else if (treatFmt == 1) {
			int temp = BasicTool.eareFind(BF[1], BB[1], SF);
			return string.substring(BF[1] + 1, SF[temp]).replace("、", ", ");
		} else {
			int temp = BasicTool.eareFind(BF[0], BB[0], SF);
			return string.substring(BF[0] + 1, SF[temp]).replace("、", ", ");
		}
	}

	/**
	 * 读取作者信息，位于带"()"的"[]"中"()"内
	 * 
	 * @param string
	 *            被读取的字符串
	 * @param treatFmt
	 *            处理格式:0="~~~"、
	 *            1="[~~~][~~~(~~~)]~~~(~~~)"、2="[~~~(~~~)]~~~(~~~)[~~~]"
	 * @param BF
	 *            中前括号位置
	 * @param BB
	 *            中后括号位置
	 * @param SF
	 *            小前括号位置
	 * @param SB
	 *            小后括号位置
	 * @return
	 */
	private static String readAuthor(String string, int treatFmt, int[] BF, int[] BB, int[] SF, int[] SB) {
		// 0="~~~"、 1="[~~~][~~~(~~~)]~~~(~~~)"、2="[~~~(~~~)]~~~(~~~)[~~~]"。
		if (treatFmt == 0) {
			return "";
		} else if (treatFmt == 1) {
			int temp = BasicTool.eareFind(BF[1], BB[1], SF);
			return string.substring(SF[temp] + 1, SB[temp]).replace("、", ", ");
		} else {
			int temp = BasicTool.eareFind(BF[0], BB[0], SF);
			return string.substring(SF[temp] + 1, SB[temp]).replace("、", ", ");
		}
	}

	/**
	 * 读取作品名信息，位于带"()"的"[]"后到最后一个"()"前
	 * 
	 * @param string
	 *            被读取的字符串
	 * @param treatFmt
	 *            处理格式:0="~~~"、
	 *            1="[~~~][~~~(~~~)]~~~(~~~)"、2="[~~~(~~~)]~~~(~~~)[~~~]"
	 * @param BB
	 *            中后括号位置
	 * @param SF
	 *            小前括号位置
	 * @return
	 */
	private static String readTitle(String string, int treatFmt, int[] BB, int[] SF) {
		// 0="~~~"、 1="[~~~][~~~(~~~)]~~~(~~~)"、2="[~~~(~~~)]~~~(~~~)[~~~]"。
		if (treatFmt == 0) {
			return string;
		} else if (treatFmt == 1) {
			int temp = BasicTool.lastNonzero(SF);
			return string.substring(BB[1] + 1, temp);
		} else {
			int temp = BasicTool.lastNonzero(SF);
			return string.substring(BB[0] + 1, temp);
		}
	}

	/**
	 * 读取原作名信息，位于最后一个"()"内，同时将其转换为标准原作名
	 * 
	 * @param string
	 *            被读取的字符串
	 * @param treatFmt
	 *            处理格式:0="~~~"、
	 *            1="[~~~][~~~(~~~)]~~~(~~~)"、2="[~~~(~~~)]~~~(~~~)[~~~]"
	 * @param SF
	 *            小前括号位置
	 * @param SB
	 *            小后括号位置
	 * @return
	 */
	private static String readOriginalTitle(String string, int treatFmt, int[] SF, int[] SB) {
		// 0="~~~"、 1="[~~~][~~~(~~~)]~~~(~~~)"、2="[~~~(~~~)]~~~(~~~)[~~~]"。
		if (treatFmt == 0) {
			return "";
		} else {
			int temp1 = BasicTool.lastNonzero(SF);
			int temp2 = BasicTool.lastNonzero(SB);
			String temp = string.substring(temp1 + 1, temp2);
			int i = 0;
			while (chgInfo[i] != null) {
				if (temp.indexOf(chgInfo[i]) != -1) {
					temp = chgInfo[i + 1];
				}
				i += 2;
			}
			return temp;
		}
	}

	/**
	 * 读取汉化组信息，treatFmt=1则位于第一个"[]"内，treatFmt=2则位于最后一个"[]"内
	 * 
	 * @param string
	 *            被读取的字符串
	 * @param treatFmt
	 *            处理格式:0="~~~"、
	 *            1="[~~~][~~~(~~~)]~~~(~~~)"、2="[~~~(~~~)]~~~(~~~)[~~~]"
	 * @param BF
	 *            中前括号位置
	 * @param BB
	 *            中后括号位置
	 * @return
	 */
	private static String readTranslator(String string, int treatFmt, int[] BF, int[] BB) {
		// 0="~~~"、 1="[~~~][~~~(~~~)]~~~(~~~)"、2="[~~~(~~~)]~~~(~~~)[~~~]"。
		if (treatFmt == 0) {
			return "";
		} else if (treatFmt == 1) {
			return string.substring(BF[0] + 1, BB[0]);
		} else {
			int temp1 = BasicTool.lastNonzero(BF);
			int temp2 = BasicTool.lastNonzero(BB);
			return string.substring(temp1 + 1, temp2);
		}
	}

	/**
	 * 根据正则表达式匹配原则，识别出散乱的字符串的排列特征，再根据各种特征，调整排列，达到便于识别的标准格式。
	 * 标准格式为(~~~)[~~~(~~~)]~~~(~~~)[~~~]
	 * 
	 * @param str
	 *            需要調整的字符串
	 * @return 处理结果，第一个是字符串处理结果;第二个是处理格式,返回"1"则运行成功，返回"0"则匹配失败。
	 */
	private static String[] PretreatmentMatcher(String str) {
		ArrayList<String> patternArray = new ArrayList<String>();
		patternArray.add("(\\(.+\\)\\[.+\\(.+\\)\\].+\\(.+\\)\\[.+\\])");
		patternArray.add("(\\[.+\\]\\(.+\\)\\[.+\\(.+\\)\\].+\\(.+\\))");
		patternArray.add("(\\[.+\\(.+\\)\\].+\\(.+\\)\\[.+\\])");
		patternArray.add("(\\(.+\\)\\[.+\\(.+\\)\\].+\\[.+\\])");
		patternArray.add("(\\(.+\\)\\[.+\\].+\\(.+\\)\\[.+\\])");
		patternArray.add("(\\[.+\\(.+\\)\\].+\\[.+\\])");
		patternArray.add("(\\[.+\\].+\\(.+\\)\\[.+\\])");
		patternArray.add("(\\(.+\\)\\[.+\\].+\\[.+\\])");
		patternArray.add("(\\[.+\\]\\[.+\\(.+\\)\\].+\\(.+\\))");
		patternArray.add("(\\[.+\\]\\(.+\\)\\[.+\\(.+\\)\\].+)");
		patternArray.add("(\\[.+\\]\\(.+\\)\\[.+\\].+\\(.+\\))");
		patternArray.add("(\\[.+\\]\\[.+\\(.+\\)\\].+)");
		patternArray.add("(\\[.+\\]\\(.+\\)\\[.+\\].+)");
		patternArray.add("(\\[.+\\]\\[.+\\].+\\(.+\\))");
		patternArray.add("(\\(.+\\)\\[.+\\(.+\\)\\].+\\(.+\\))");
		patternArray.add("(\\[.+\\(.+\\)\\].+\\(.+\\))");
		patternArray.add("(\\(.+\\)\\[.+\\(.+\\)\\].+)");
		patternArray.add("(\\(.+\\)\\[.+\\].+\\(.+\\))");
		patternArray.add("(\\[.+\\(.+\\)\\].+)");
		patternArray.add("(\\(.+\\)\\[.+\\].+)");
		patternArray.add("(\\[.+\\].+\\(.+\\))");
		patternArray.add("(\\(.+\\).+\\(.+\\))");
		patternArray.add("(.+\\(.+\\))");
		patternArray.add("(\\(.+\\).+)");
		patternArray.add("(.+)");
		int fmt = 0;
		String[] rslt = new String[2];
		rslt[1] = String.valueOf(fmt);

		for (int j = 0; j < patternArray.size(); j++) {
			Pattern r = Pattern.compile(patternArray.get(j));
			Matcher m = r.matcher(str);
			if (m.find()) {
				// 根据匹配的格式，添加缺失的格式符号
				switch (j) {
				case 0:
					fmt = 1;
					break;
				case 1:
					fmt = 2;
					break;
				case 2:
					str = BasicTool.stringInsert(str, "()", str.indexOf("["));
					fmt = 1;
					break;
				case 3:
					str = BasicTool.stringInsert(str, "()", str.lastIndexOf("["));
					fmt = 1;
					break;
				case 4:
					str = BasicTool.stringInsert(str, "()", str.indexOf("]"));
					fmt = 1;
					break;
				case 5:
					str = BasicTool.stringInsert(str, "()", str.indexOf("["));
					str = BasicTool.stringInsert(str, "()", str.lastIndexOf("["));
					fmt = 1;
					break;
				case 6:
					str = BasicTool.stringInsert(str, "()", str.indexOf("["));
					str = BasicTool.stringInsert(str, "()", str.indexOf("]"));
					fmt = 1;
					break;
				case 7:
					str = BasicTool.stringInsert(str, "()", str.indexOf("]"));
					str = BasicTool.stringInsert(str, "()", str.lastIndexOf("["));
					fmt = 1;
					break;
				case 8:
					str = BasicTool.stringInsert(str, "()", str.indexOf("]") + 1);
					fmt = 2;
					break;
				case 9:
					str = BasicTool.stringInsert(str, "()", str.length());
					fmt = 2;
					break;
				case 10:
					str = BasicTool.stringInsert(str, "()", str.indexOf("]", str.indexOf("]") + 1));
					fmt = 2;
					break;
				case 11:
					str = BasicTool.stringInsert(str, "()", str.indexOf("]") + 1);
					str = BasicTool.stringInsert(str, "()", str.length());
					fmt = 2;
					break;
				case 12:
					str = BasicTool.stringInsert(str, "()", str.indexOf("]", str.indexOf("]") + 1));
					str = BasicTool.stringInsert(str, "()", str.length());
					fmt = 2;
					break;
				case 13:
					str = BasicTool.stringInsert(str, "()", str.indexOf("]") + 1);
					str = BasicTool.stringInsert(str, "()", str.indexOf("]", str.indexOf("]") + 1));
					fmt = 2;
					break;
				case 14:
					fmt = 3;
					break;
				case 15:
					str = BasicTool.stringInsert(str, "()", str.indexOf("["));
					fmt = 3;
					break;
				case 16:
					str = BasicTool.stringInsert(str, "()", str.length());
					fmt = 3;
					break;
				case 17:
					str = BasicTool.stringInsert(str, "()", str.indexOf("]"));
					fmt = 3;
					break;
				case 18:
					str = BasicTool.stringInsert(str, "()", str.indexOf("["));
					str = BasicTool.stringInsert(str, "()", str.length());
					fmt = 3;
					break;
				case 19:
					str = BasicTool.stringInsert(str, "()", str.indexOf("]"));
					str = BasicTool.stringInsert(str, "()", str.length());
					fmt = 3;
					break;
				case 20:
					str = BasicTool.stringInsert(str, "()", str.indexOf("["));
					str = BasicTool.stringInsert(str, "()", str.indexOf("]"));
					fmt = 3;
					break;
				case 21:
					fmt = 4;
					break;
				case 22:
					str = BasicTool.stringInsert(str, "()", 0);
					fmt = 4;
					break;
				case 23:
					str = BasicTool.stringInsert(str, "()", str.length());
					fmt = 4;
					break;
				case 24:
					str = BasicTool.stringInsert(str, "()", 0);
					str = BasicTool.stringInsert(str, "()", str.length());
					fmt = 4;
					break;
				default:
					break;
				}
				// 进一步修改符号调整为标准格式
				if (fmt == 4) {
					str = BasicTool.stringInsert(str, "[()]", str.indexOf(")") + 1);
					fmt = 3;
				}
				if (fmt == 3) {
					str = BasicTool.stringInsert(str, "[]", str.length());
					fmt = 1;
				}
				if (fmt == 2) {
					String temp = str.substring(str.indexOf("["), str.indexOf("]") + 1);
					str = str.replace(temp, "");
					str = BasicTool.stringInsert(str, temp, str.length());
					fmt = 1;
				}
				System.out.println(str);
				break;
			}
		}

		rslt[0] = str;
		rslt[1] = String.valueOf(fmt);
		return rslt;
	}

	/**
	 * 根据标准格式的字符串，由正则表达式匹配原则，得到字符串中特定的信息。
	 * 
	 * @param str
	 *            标准格式的字符串
	 * @return rslt[1]:时间、[2]:制作方名、[3]:作者名、[4]:作品名、[5]:原作名、[6]:汉化组名、[7]:标准化结果
	 */
	private static String[] readInfo(String str) {
		String[] rslt = new String[7];
		String pattern = "(\\([^\\(\\]]*\\))(\\[[^\\(\\]]*)(\\([^\\(\\]]*\\)\\])([^\\(\\]]*)(\\([^\\(\\]]*\\))(\\[[^\\(\\]]*\\])";

		Pattern r = Pattern.compile(pattern);
		Matcher m = r.matcher(str);
		m.find();
		rslt[1] = m.group(1).substring(1, m.group(1).length() - 1);
		rslt[2] = m.group(2).substring(1, m.group(2).length());
		rslt[3] = m.group(3).substring(1, m.group(3).length() - 2);
		rslt[4] = m.group(4);
		rslt[5] = m.group(5).substring(1, m.group(5).length() - 1);
		rslt[6] = m.group(6).substring(1, m.group(6).length() - 1);
		
		int i = 0;
		while (chgInfo[i] != null) {
			if (rslt[5].indexOf(chgInfo[i]) != -1) {
				rslt[5] = chgInfo[i + 1];
			}
			i += 2;
		}
		
		return rslt;
	}

	/**
	 * 从某种格式的文件名中提取制作方名、作者名、作品名、原作名和汉化组名的信息。 并前期重新排列成以下标准格式：
	 * "标记[制作方名 (作者名)] 作品名 (原作名) [汉化组名]"。加入了单次处理，单次回应的功能，使用一次处理一个文件夹，并返回处理结果。
	 * 先对字符串进行预处理，使之成为半标准的名称；再以此提取上述信息组件；最后根据需要重新排列信息组件。
	 * component[0]:标记、[1]:时间、[2]:制作方名、[3]:作者名、[4]:作品名、[5]:原作名、[6]:汉化组名、[7]:
	 * 标准化结果
	 * 
	 * @return 标准格式的字符串
	 */
	private static String analyzeName() {
		int xltUnkwn = 0;
		String string = fldrName[nowFldr];
		String[] component = new String[10];
		if (debugMode == 1) {
			System.out.println("Origin:");
			System.out.println(string);
		}

		string = stringReplacement(string);
		if (string.indexOf("[中国翻訳]") != -1) {
			string = string.replace("[中国翻訳]", "");
			xltUnkwn = 1;
		}
		if (string.indexOf("[Chinese]") != -1) {
			string = string.replace("[Chinese]", "");
			xltUnkwn = 1;
		}
		if (xltUnkwn == 1) {
			component[0] = "★";
		} else {
			component[0] = "";
		}
		if (string.indexOf("★") != -1) {
			component[0] = "★";
		}
		// int[] BF = BasicTool.findAll(string, "[");
		// int[] BB = BasicTool.findAll(string, "]");
		// int[] SF = BasicTool.findAll(string, "(");
		// int[] SB = BasicTool.findAll(string, ")");
		if (debugMode == 1) {
			System.out.println("Replacement:");
			System.out.println(string);
			// System.out.print("BF: ");
			// BasicTool.show(BF);
			// System.out.print("BB: ");
			// BasicTool.show(BB);
			// System.out.print("SF: ");
			// BasicTool.show(SF);
			// System.out.print("SB: ");
			// BasicTool.show(SB);
			System.out.print("Length: ");
			System.out.println(string.length());
		}
		// if (bracketTesting(BF, BB, SF, SB) == 1) {
		// errCode[nowFldr] = 1;
		// return null;
		// }

		try {
			// String[] tempString = Pretreatment(string, BF, BB, SF, SB);
			String[] tempString = PretreatmentMatcher(string);
			string = tempString[0];
			int rslt = Integer.parseInt(tempString[1]);
		} catch (Exception e) {
			errCode[nowFldr] = 6;
			return string;
		}
		// BF = BasicTool.findAll(string, "[");
		// BB = BasicTool.findAll(string, "]");
		// SF = BasicTool.findAll(string, "(");
		// SB = BasicTool.findAll(string, ")");
		if (debugMode == 1) {
			System.out.println("Pretreatment:");
			// System.out.print("BF: ");
			// BasicTool.show(BF);
			// System.out.print("BB: ");
			// BasicTool.show(BB);
			// System.out.print("SF: ");
			// BasicTool.show(SF);
			// System.out.print("SB: ");
			// BasicTool.show(SB);
			System.out.print("Length: ");
			System.out.println(string.length());
		}

		// try {
		// component[1] = readProducers(string, treatFmt, BF, BB, SF);
		// } catch (Exception e) {
		// errCode[nowFldr] = 2;
		// }
		// try {
		// component[2] = readAuthor(string, treatFmt, BF, BB, SF, SB);
		// } catch (Exception e) {
		// errCode[nowFldr] = 3;
		// }
		// try {
		// component[3] = readTitle(string, treatFmt, BB, SF);
		// } catch (Exception e) {
		// errCode[nowFldr] = 4;
		// }
		// try {
		// component[4] = readOriginalTitle(string, treatFmt, SF, SB);
		// } catch (Exception e) {
		// errCode[nowFldr] = 5;
		// }
		// try {
		// component[5] = readTranslator(string, treatFmt, BF, BB);
		// } catch (Exception e) {
		// errCode[nowFldr] = 6;
		// }
		try {
			String[] temp = readInfo(string);
			component[1] = temp[1];
			component[2] = temp[2];
			component[3] = temp[3];
			component[4] = temp[4];
			component[5] = temp[5];
			component[6] = temp[6];
		} catch (Exception e) {
			errCode[nowFldr] = 12;
			return string;
		}

		if (component[0] == "★" && component[6].length() != 0) {
			component[0] = "";
		}
		if (debugMode == 1) {
			System.out.println("Extract component:");
			BasicTool.show(component);
		}

		/*
		 * component[7] = ""; if (component[0].length() != 0) { component[7] =
		 * component[7] + component[0] + " "; } if (stdFmt != 0) { if (stdFmt ==
		 * 2) { if (component[6].length() != 0) { component[7] = component[7] +
		 * "[" + component[6] + "] "; } } if (component[1].length() != 0) {
		 * component[7] = component[7] + "(" + component[1] + ") "; } } if
		 * (component[2].length() != 0) { if (component[3].length() != 0) {
		 * component[7] = component[7] + "[" + component[2] + " (" +
		 * component[3] + ")]"; } else { component[7] = component[7] + "[" +
		 * component[2] + "]"; } } if (component[4].length() != 0) { if
		 * (component[7].length() != 0) { component[7] = component[7] + " " +
		 * component[4]; } else { component[7] = component[7] + component[4]; }
		 * } if (component[5].length() != 0) { component[7] = component[7] +
		 * " (" + component[5] + ")"; } if (component[6].length() != 0) {
		 * component[7] = component[7] + " [" + component[6] + "]"; }
		 */

		switch (stdFmt) {
		case 0:
			component[7] = component[0] + " [" + component[2] + " (" + component[3] + ")] " + component[4] + " ("
					+ component[5] + ") [" + component[6] + "]";
			break;
		case 1:
			component[7] = component[0] + " (" + component[1] + ") [" + component[2] + " (" + component[3] + ")] "
					+ component[4] + " (" + component[5] + ") [" + component[6] + "]";
			break;
		case 2:
			component[7] = component[0] + " [" + component[6] + "] (" + component[1] + ") [" + component[2] + " ("
					+ component[3] + ")] " + component[4] + " (" + component[5] + ")";
			break;
		default:
			break;
		}

		component[7] = stringReplacement2(component[7]);
		component[7] = component[7].trim();

		if (debugMode == 1) {
			System.out.println("Analyze result:");
			System.out.println(component[7] + "\n");
		}
		return component[7];
	}

	/**
	 * 使用标准格式的名称替换原始名称，并返回工作结果。 根据错误标记，返回错误信息。
	 * 
	 * @param oldName
	 *            原始名称
	 * @param newName
	 *            标准名称
	 * @return 工作结果说明
	 */
	private static String rename(String oldName, String newName) {
		String workInfo = null;
		if (errCode[nowFldr] == 0) {
			if (oldName.equals(newName)) {
				workInfo = oldName + "\ndo not need to rename.";
				workState = 10;
			} else {
				cmplteAmt[0]++;
				try {
					FileOperate.moveFolder2(srcPath + "\\" + oldName, srcPath + "\\" + newName);
					cmplteAmt[1]++;
					workInfo = oldName + "\nhas been renamed to:\n" + newName;
				} catch (Exception e) {
					workInfo = oldName + "\nrename failed.";
				}
			}
		} else if (errCode[nowFldr] == 1) {
			workInfo = oldName + "\nbrackets inspection failed.";
		} else if (errCode[nowFldr] == 2) {
			workInfo = oldName + "\nread producers filed.";
		} else if (errCode[nowFldr] == 3) {
			workInfo = oldName + "\nread author filed.";
		} else if (errCode[nowFldr] == 4) {
			workInfo = oldName + "\nread title filed.";
		} else if (errCode[nowFldr] == 5) {
			workInfo = oldName + "\nread original tille filed.";
		} else if (errCode[nowFldr] == 6) {
			workInfo = oldName + "\nread translator filed.";
		} else if (errCode[nowFldr] == 11) {
			workInfo = oldName + "\nregular expression preprocess failed.";
		} else if (errCode[nowFldr] == 12) {
			workInfo = oldName + "\nregular expression read information filed.";
		}

		workInfo = workInfo + "\n\n";
		return workInfo;
	}

	/**
	 * 名称标准化主函数。 通过反复调用实现功能。每次调用根据工作状态标记进行相应工作。workState =
	 * 0:标准化数据准备开始、1:标准化数据准备完毕、2:标准化工作开始、3:标准化工作中、4:标准化工作结束
	 * 
	 * @return 工作结果说明
	 */
	public static String nameStandardization() {
		if (workState == 0) {
			return dataCollectionStandardization();
		} else if (workState == 1) {
			workState = 2;
			return "┏━━━━━(`·ω·′) Name standardizing work start━━━━━━┓\n\n";
		} else if (fldrEfflAmt == 0) {
			workState = 4;
			return "┗━━━━━━━━∑(゜ロ゜;)!! No folders exist━━━━━━━━┛\n";
		} else {
			workState = 3;
			nowFldr++;
		}

		if (nowFldr == fldrEfflAmt && workState == 3) {
			workState = 4;
			return "┗━━━( ´･Д･)ゞ Total " + cmplteAmt[1] + " in " + fldrEfflAmt + " files standardized well━━┛\n";
		}

		if (workState == 3) {
			stdFldrName[nowFldr] = analyzeName();
			return rename(fldrName[nowFldr], stdFldrName[nowFldr]);
		}

		return "";
	}

	/**
	 * 撤销nameStandardization的操作，返回处理结果
	 * 
	 * @return 工作结果
	 */
	public String revocation() {
		if (workState == 4) {
			workState = 2;
			nowFldr = -1;
			cmplteAmt[0] = 0;
			cmplteAmt[1] = 0;
			return "\n┏━━━（´_ゝ`） Name standardization revocation start━━━┓\n\n";
		} else {
			nowFldr++;
			workState = 3;
		}

		if (nowFldr == fldrEfflAmt) {
			workState = 0;
			return "┗━━━( ´･Д･)ゞ Total " + cmplteAmt[0] + " in " + cmplteAmt[1] + " files revocated well━━━┛\n";
		}

		if (workState == 3) {
			if (errCode[nowFldr] == 0) {
				return rename(stdFldrName[nowFldr], fldrName[nowFldr]);
			} else {
				return rename(fldrName[nowFldr], "");
			}
		}

		return "";
	}

	/**
	 * 删除文件夹内特定碎片文件
	 * 
	 * @param srcFileName
	 */
	public static int deletefile(String[][] srcFileName) {

		String[] feature = { ".torrent", "Thumbs.db", ".xehdone", ".url" };
		int deleted = 0;
		for (int i = 0; i < fldrEfflAmt; i++) {
			for (int j = 0; j < fileEfflAmtPerFolder[i]; j++) {
				for (int k = 0; k < feature.length; k++) {
					if (srcFileName[i][j].indexOf(feature[k]) != -1) {
						FileOperate.delFile(srcPath + "\\" + fldrName[i] + "\\" + srcFileName[i][j]);
						fileEfflAmtPerFolder[i]--;
						deleted = 1;
					}
				}
			}
		}
		return deleted;
	}

	/**
	 * 计算两个二维数组每项之差，结果用二维数组返回
	 * 
	 * @param intGroup1
	 *            被减二维数组
	 * 
	 * @param intGroup2
	 *            减二维数组
	 * 
	 * @return 相减结果
	 */
	private static long[][] sizeGap(long[][] intGroup1, long[][] intGroup2) {
		long[][] srcBigerAim = new long[fldrEfflAmt][fileMaxAmtPerFolder];

		for (int i = 0; i < fldrEfflAmt; i++) {
			for (int j = 0; j < fileEfflAmtPerFolder[i]; j++) {
				if (intGroup2[i][j] == 0) {
					errCode[i] = 8;
					break;
				} else {
					srcBigerAim[i][j] = intGroup2[i][j] > 0 ? intGroup1[i][j] - intGroup2[i][j] : 0;
				}
			}
		}
		return srcBigerAim;
	}

	/**
	 * 比较一个二维数组的每项是否大于另一个数组的每项，结果用二维数组返回
	 * 
	 * @param intGroup1
	 *            分子二维数组
	 * 
	 * @param intGroup2
	 *            分母二维数组
	 * 
	 * @return 比较结果
	 */
	private static long[][] sizeCompare(long[][] intGroup1, long[][] intGroup2) {
		long[][] bigEnuff = new long[fldrEfflAmt][fileMaxAmtPerFolder];
		for (int i = 0; i < fldrEfflAmt; i++) {
			for (int j = 0; j < fileEfflAmtPerFolder[i]; j++) {
				if (errCode[i] == 8) {
					break;
				} else {
					bigEnuff[i][j] = 100 * intGroup1[i][j] / intGroup2[i][j] > sizeThrshld ? 1 : 0;
				}
			}
		}
		return bigEnuff;
	}

	/**
	 * 系统初始化，收集剪切需要的数据 初始变量包括开始工作标记，当前工作文件夹标记，完成计数，工作状态标记，错误标记，文件大小差，文件压缩足够标记。
	 * 数据包括源文件夹名称，目标文件夹名称，文件夹有效数量，文件有效数量，源文件大小，目标文件大小。
	 * 
	 * @return 如果该初始化失败则返回失败信息。
	 */
	private static String dataCollectionShift() {
		try {
			errCode = new int[fldrEfflAmt];
			fldrName = FileOperate.readfolderName(srcPath);
			srcFileName = FileOperate.readFileName(srcPath, fldrName);
			fileEfflAmtPerFolder = FileOperate.realStringSetLength(srcFileName, fldrEfflAmt);
			if (deletefile(srcFileName) == 1) {
				srcFileName = FileOperate.readFileName(srcPath, fldrName);
			}
			aimFileName = FileOperate.readFileName(aimPath, fldrName);
			srcFileSize = FileOperate.readFileSize(srcPath, fldrName, srcFileName);// 图片修改后格式可能发生改变，此处难以用aimFileName达到相同顺序的索引
			aimFileSize = FileOperate.readFileSize(aimPath, fldrName, aimFileName);
			fldrEfflAmt = FileOperate.realStringSetLength(fldrName);
			srcBigerAim = sizeGap(srcFileSize, aimFileSize);
			bigEnuff = sizeCompare(srcBigerAim, srcFileSize);
			cmplteAmt[0] = 0;
			cmplteAmt[1] = 0;
			cmplteAmt[2] = 0;
			cmplteAmt[3] = 0;
			workState = 6;
			nowFile = -1;
			nowFldr = 0;
			temp = 0;
		} catch (Exception e) {
			errCode[0] = 10;
			return "\nヾ(ﾟдﾟ)ﾉ゛ Data collection for shift failed.\n(｀・-・)=3 Try to check the Aim Folder\n";
		}
		return "";
	}

	/**
	 * 比较源头与目标两组文件的大小，若某个目标文件大小小于源头对应文件的一定百分比，则用该目标文件替换源头文件。 返回工作结果。
	 * 根据错误标记，返回错误信息。
	 * 
	 * @return 工作结果说明
	 */
	private static String shift() {
		String srcPathTotal;
		String aimPathTotal;
		String aimPath2source;
		if (errCode[nowFldr] == 8) {
			temp = 1;
			nowFile = fileEfflAmtPerFolder[nowFldr] - 1;
			return fldrName[nowFldr] + "\nfiles amount does not match.\n\n";
		} else if (bigEnuff[nowFldr][nowFile] == 1) {
			if (srcFileName[nowFldr][nowFile].indexOf("gif") == -1) {
				srcPathTotal = srcPath + "\\" + fldrName[nowFldr] + "\\" + srcFileName[nowFldr][nowFile];
				aimPathTotal = aimPath + "\\" + fldrName[nowFldr] + "\\" + aimFileName[nowFldr][nowFile];
				aimPath2source = srcPathTotal.replace(srcPathTotal.substring(
						srcPathTotal.indexOf(".", srcPathTotal.length() - 5), srcPathTotal.length()), ".jpg");
				try {
					FileOperate.cutFile(aimPathTotal, srcPathTotal, aimPath2source);
					cmplteAmt[0]++;
					cmplteAmt[1] += srcBigerAim[nowFldr][nowFile];
				} catch (Exception e) {
					errCode[nowFldr] = 7;
					return srcFileName[nowFldr][nowFile] + "\nShift failed.\n";
				}
			}
		}
		return "";
	}

	/**
	 * 文件转移主函数。 通过反复调用实现功能。每次调用根据工作状态标记进行相应工作。workState =
	 * 5:转移数据准备开始、6:转移数据准备完毕、7:转移工作开始、8:转移工作中、9:转移工作结束 “temp = 1”表示该进入下一个文件夹
	 * 
	 * @return 结果描述
	 */
	public static String fileShift() {
		if (workState == 4) {
			workState = 5;
			return dataCollectionShift();
		} else if (workState == 6) {
			workState = 7;
			return "\n┏━━━━━━━━━(｡>д<)ｼ File shift start━━━━━━━━━┓\n\n";
		} else if (BasicTool.sum(BasicTool.sum(aimFileSize)) == 0) {
			workState = 9;
			return "┗━━━━━━━━∑(゜ロ゜;)!! No folders exist━━━━━━━━┛\n";
		}

		if (nowFldr + 1 == fldrEfflAmt && temp == 1) {
			nowFldr++;
			workState = 9;
			return "     ( ´･Д･)ゞ Total " + cmplteAmt[2] + " files in " + fldrEfflAmt + " folders shifted well\n"
					+ "┗━━━━━━━━Decreasing " + cmplteAmt[3] / 1048576 + " MB totally.━━━━━━━━━┛\n";
		}

		if (nowFile + 1 == fileEfflAmtPerFolder[nowFldr] && temp == 0) {
			cmplteAmt[2] += cmplteAmt[0];
			cmplteAmt[3] += cmplteAmt[1];
			temp = 1;
			if (debugMode == 2) {
				System.out.println(nowFldr + ":");
				System.out.println(fldrName[nowFldr]);
			}
			if (cmplteAmt[1] != 0) {
				return fldrName[nowFldr] + "\nTotal " + cmplteAmt[0] + " in " + fileEfflAmtPerFolder[nowFldr]
						+ " have been shifted, decreasing " + cmplteAmt[1] / 1048576 + " MB.\n\n";
			} else {
				workState = 10;
				return fldrName[nowFldr] + "\ndo not need be shifted.\n\n";
			}
		} else {
			if (temp == 1) {
				nowFldr++;
				nowFile = -1;
				temp = 0;
			}
			nowFile += 1;
			workState = 8;
			if (nowFile == 0) {
				cmplteAmt[0] = 0;
				cmplteAmt[1] = 0;
			}
		}

		if (workState == 8) {
			return shift();
		}

		return "";
	}

	public static void main(String[] args) {

		srcPath = "E:\\Temporary\\1Check\\2Check\\(Batch)";
		aimPath = "E:\\Temporary\\新建文件夹";

		if (sglMode == 1) {
			fldrName = FileOperate.readfolderName(srcPath);
			fldrEfflAmt = FileOperate.realStringSetLength(fldrName);
			errCode = new int[fldrEfflAmt];
			srcFileName = FileOperate.readFileName(srcPath, fldrName);
			fileEfflAmtPerFolder = FileOperate.realStringSetLength(srcFileName, fldrEfflAmt);
			// aimFileName = FileOperate.readFileName(aimPath, fldrName);
			srcFileSize = FileOperate.readFileSize(srcPath, fldrName, srcFileName);
			// aimFileSize = FileOperate.readFileSize(aimPath, fldrName,
			// aimFileName);
			// srcBigerAim = sizeGap(srcFileSize, aimFileSize);
			// bigEnuff = sizeCompare(srcBigerAim, srcFileSize);
			BasicTool.show(fldrName);
			System.out.println(fldrEfflAmt);
			BasicTool.show(srcFileSize);
			// BasicTool.show(aimFileSize);
			// BasicTool.show(srcBigerAim);
			// BasicTool.show(bigEnuff);

		} else {
			/**
			 * 标准化命名调用范例
			 */
			while (workState != 4) {
				String workInfo = nameStandardization();
				if (ShowRslt == 1) {
					System.out.print(workInfo);
				}
			}

			/**
			 * 文件替换调用范例
			 */
			/*
			 * while (workState != 9) { String workInfo = fileShift(); if
			 * (ShowRslt == 1) { System.out.print(workInfo); } }
			 */

		}

	}
}

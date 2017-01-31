import java.util.ArrayList;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class test {
	public static void main(String args[]) {
		String line = "(C91)[てごねスパイク(弱揃)]おとなのYOUSOLO!(ラブライブ! サンシャイン!!)[]";

		String[] rlst = new String[7];
		String pattern = "(\\([^\\(\\]]*\\))(\\[[^\\(\\]]*)(\\([^\\(\\]]*\\)\\])([^\\(\\]]*)(\\([^\\(\\]]*\\))(\\[[^\\(\\]]*\\])";

		Pattern r = Pattern.compile(pattern);
		Matcher m = r.matcher(line);
		if (m.find()) {
			for (int i = 0; i <= m.groupCount(); i++) {
				System.out.println("Found value: " + m.group(i));
			}

		} else {
			System.out.println("NO MATCH");
		}

		rlst[1] = m.group(1).substring(1, m.group(1).length() - 1);
		rlst[2] = m.group(2).substring(1, m.group(2).length());
		rlst[3] = m.group(3).substring(1, m.group(3).length() - 2);
		rlst[4] = m.group(4);
		rlst[5] = m.group(5).substring(1, m.group(5).length() - 1);
		rlst[6] = m.group(6).substring(1, m.group(6).length() - 1);
		for (int i = 0; i < 7; i++) {
			System.out.println("value: " + rlst[i]);
		}
	}

}

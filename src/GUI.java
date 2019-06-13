import java.awt.Color;
import java.awt.Dimension;
import java.awt.EventQueue;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.JButton;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JTextPane;
import javax.swing.JTextField;
import javax.swing.JProgressBar;
import javax.swing.JScrollPane;
import javax.swing.SwingConstants;
import java.awt.Font;
import javax.swing.JSpinner;
import javax.swing.SpinnerNumberModel;
import javax.swing.text.BadLocationException;
import javax.swing.text.Style;
import javax.swing.text.StyleConstants;
import java.awt.Toolkit;
import javax.swing.border.CompoundBorder;
import javax.swing.border.MatteBorder;
import javax.swing.JPanel;
import javax.swing.JComboBox;
import javax.swing.DefaultComboBoxModel;

public class GUI {

	private JFrame frmAsdasd;
	private JTextField txtE;
	private JTextField txtE_1;
	private JTextField txtEtooleditorformatfactory;
	PendingFile Pending = new PendingFile();
	Thread thread1 = new Thread();

	/**
	 * Launch the application.
	 */
	public static void main(String[] args) {
		EventQueue.invokeLater(new Runnable() {
			public void run() {
				try {
					GUI window = new GUI();
					window.frmAsdasd.setVisible(true);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
	}

	/**
	 * Create the application.
	 */
	public GUI() {
		initialize();
	}

	/**
	 * Initialize the contents of the frame.
	 */
	private void initialize() {
		frmAsdasd = new JFrame();
		frmAsdasd.setTitle("同人志文件处理器_v2.4 Present by Sandiagle");
		frmAsdasd.setBackground(Color.LIGHT_GRAY);
		frmAsdasd.getContentPane().setBackground(Color.LIGHT_GRAY);
		frmAsdasd.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frmAsdasd.getContentPane().setLayout(null);
		Toolkit toolkit = Toolkit.getDefaultToolkit();
		Dimension scrnsize = toolkit.getScreenSize();
		int frameWidth = 600;
		int frameHeight = 900;
		frmAsdasd.setBounds(scrnsize.width * 1 / 4 - frameWidth / 2, scrnsize.height / 2 - frameHeight / 2, frameWidth,
				frameHeight);

		JLabel Label_Source = new JLabel("\u6E90\u5934\u76EE\u5F55\u8DEF\u5F84");
		Label_Source.setFont(new Font("方正准圆_GBK", Font.PLAIN, 18));
		Label_Source.setForeground(Color.BLACK);
		Label_Source.setHorizontalAlignment(SwingConstants.CENTER);
		Label_Source.setBounds(14, 15, 125, 27);
		frmAsdasd.getContentPane().add(Label_Source);

		txtE = new JTextField();
		txtE.setText("E:\\Temporary\\1Check\\2Check\\(Batch)");
		txtE.setBounds(153, 16, 300, 24);
		frmAsdasd.getContentPane().add(txtE);
		txtE.setColumns(10);

		JButton button_Source = new JButton("<html>选择路径");
		button_Source.setFont(new Font("方正准圆_GBK", Font.PLAIN, 15));
		button_Source.setToolTipText("");
		button_Source.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				JFileChooser jfc = new JFileChooser();
				jfc.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);
				jfc.showDialog(new JLabel(), "Choose");
				txtE.setText(jfc.getSelectedFile().getPath());
			}
		});
		button_Source.setBounds(466, 14, 100, 27);
		frmAsdasd.getContentPane().add(button_Source);

		JLabel label_Aim = new JLabel("\u76EE\u6807\u76EE\u5F55\u8DEF\u5F84");
		label_Aim.setForeground(Color.BLACK);
		label_Aim.setFont(new Font("方正准圆_GBK", Font.PLAIN, 18));
		label_Aim.setHorizontalAlignment(SwingConstants.CENTER);
		label_Aim.setBounds(14, 56, 125, 27);
		frmAsdasd.getContentPane().add(label_Aim);

		txtE_1 = new JTextField();
		txtE_1.setText("E:\\Temporary\\新建文件夹");
		txtE_1.setColumns(10);
		txtE_1.setBounds(153, 57, 300, 24);
		frmAsdasd.getContentPane().add(txtE_1);

		JButton button_Aim = new JButton("<html>选择路径");
		button_Aim.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				JFileChooser jfc = new JFileChooser();
				jfc.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);
				jfc.showDialog(new JLabel(), "Choose");
				txtE_1.setText(jfc.getSelectedFile().getPath());
			}
		});
		button_Aim.setToolTipText("");
		button_Aim.setFont(new Font("方正准圆_GBK", Font.PLAIN, 15));
		button_Aim.setBounds(466, 55, 100, 27);
		frmAsdasd.getContentPane().add(button_Aim);

		JLabel Label_GraphicExe = new JLabel("\u56FE\u50CF\u5904\u7406\u8DEF\u5F84");
		Label_GraphicExe.setForeground(Color.BLACK);
		Label_GraphicExe.setFont(new Font("方正准圆_GBK", Font.PLAIN, 18));
		Label_GraphicExe.setHorizontalAlignment(SwingConstants.CENTER);
		Label_GraphicExe.setBounds(14, 96, 125, 27);
		frmAsdasd.getContentPane().add(Label_GraphicExe);

		txtEtooleditorformatfactory = new JTextField();
		txtEtooleditorformatfactory.setText("E:\\Tool\\Editor\\FormatFactory\\FormatFactory.exe");
		txtEtooleditorformatfactory.setColumns(10);
		txtEtooleditorformatfactory.setBounds(153, 97, 300, 24);
		frmAsdasd.getContentPane().add(txtEtooleditorformatfactory);

		JButton button_GraphicExe = new JButton("<html>选择路径");
		button_GraphicExe.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				JFileChooser jfc = new JFileChooser();
				jfc.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);
				jfc.showDialog(new JLabel(), "Choose");
				txtEtooleditorformatfactory.setText(jfc.getSelectedFile().getPath());
			}
		});
		button_GraphicExe.setToolTipText("");
		button_GraphicExe.setFont(new Font("方正准圆_GBK", Font.PLAIN, 15));
		button_GraphicExe.setBounds(466, 95, 100, 27);
		frmAsdasd.getContentPane().add(button_GraphicExe);

		JPanel panel_2 = new JPanel();
		panel_2.setBorder(new MatteBorder(1, 1, 1, 1, (Color) new Color(0, 0, 0)));
		panel_2.setBackground(Color.LIGHT_GRAY);
		panel_2.setBounds(9, 10, 562, 118);
		frmAsdasd.getContentPane().add(panel_2);

		JTextPane textAreatextWorkInfo = new JTextPane();
		textAreatextWorkInfo.setSize(47, 152);
		textAreatextWorkInfo.setFont(new Font("Monospaced", Font.PLAIN, 15));
		// textAreatextWorkInfo.setFont(new Font("方正准圆_GBK", Font.PLAIN, 13));
		textAreatextWorkInfo.setLocation(-437, -237);
		// textAreatextWorkInfo.setLineWrap(true);
		frmAsdasd.getContentPane().add(textAreatextWorkInfo);

		Style def = textAreatextWorkInfo.getStyledDocument().addStyle(null, null);
		StyleConstants.setFontFamily(def, "Monospaced");
		StyleConstants.setFontSize(def, 15);
		Style normal = textAreatextWorkInfo.addStyle("normal", def);
		Style blue = textAreatextWorkInfo.addStyle("blue", normal);
		StyleConstants.setForeground(blue, Color.BLUE);
		Style red = textAreatextWorkInfo.addStyle("red", normal);
		StyleConstants.setForeground(red, Color.RED);
		Style orange = textAreatextWorkInfo.addStyle("orange", normal);
		StyleConstants.setForeground(orange, Color.ORANGE);
		Style magenta = textAreatextWorkInfo.addStyle("magenta", normal);
		StyleConstants.setForeground(magenta, Color.MAGENTA);
		Style gray = textAreatextWorkInfo.addStyle("gray", normal);
		StyleConstants.setForeground(gray, Color.GRAY);
		textAreatextWorkInfo.setParagraphAttributes(normal, true);

		JScrollPane scroll = new JScrollPane(textAreatextWorkInfo);
		scroll.setBounds(14, 416, 552, 385);
		frmAsdasd.getContentPane().add(scroll);

		JProgressBar progressBar_Standize = new JProgressBar();
		progressBar_Standize.setBounds(153, 202, 300, 14);
		frmAsdasd.getContentPane().add(progressBar_Standize);
		JButton button_Revocation = new JButton("<html>撤销");
		button_Revocation.setFont(new Font("方正准圆_GBK", Font.PLAIN, 15));
		JButton button_Graphic = new JButton("<html>图像处理");
		button_Graphic.setFont(new Font("方正准圆_GBK", Font.PLAIN, 20));
		button_Graphic.setEnabled(false);
		JButton button_Shift = new JButton("<html>文件替换");
		button_Shift.setFont(new Font("方正准圆_GBK", Font.PLAIN, 20));
		button_Shift.setEnabled(false);
		JButton button_restart = new JButton("<html>重开");
		JComboBox comboBox = new JComboBox();
		comboBox.setFont(new Font("方正准圆_GBK", Font.PLAIN, 15));
		comboBox.setModel(new DefaultComboBoxModel(new String[] {
				"\u6807\u8BB0 [\u5236\u4F5C\u65B9\u540D (\u4F5C\u8005\u540D)] \u4F5C\u54C1\u540D (\u539F\u4F5C\u540D) [\u6C49\u5316\u7EC4\u540D]",
				"\u6807\u8BB0 (\u65F6\u95F4) [\u5236\u4F5C\u65B9\u540D (\u4F5C\u8005\u540D)] \u4F5C\u54C1\u540D (\u539F\u4F5C\u540D) [\u6C49\u5316\u7EC4\u540D]",
				"\u6807\u8BB0 [\u6C49\u5316\u7EC4\u540D] (\u65F6\u95F4) [\u5236\u4F5C\u65B9\u540D (\u4F5C\u8005\u540D)] \u4F5C\u54C1\u540D (\u539F\u4F5C\u540D)" }));
		comboBox.setBounds(153, 177, 413, 21);
		frmAsdasd.getContentPane().add(comboBox);

		JButton btnnbsp = new JButton("<html>&thinsp&thinsp名称<br>标准化");
		btnnbsp.setBorder(new CompoundBorder());
		btnnbsp.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent arg0) {
				new ThreadStandize(textAreatextWorkInfo, Pending, progressBar_Standize, button_Revocation, btnnbsp,
						button_Graphic, button_Source, button_restart, txtE, comboBox).start();
			}
		});
		btnnbsp.setFont(new Font("方正准圆_GBK", Font.PLAIN, 20));
		btnnbsp.setBounds(14, 136, 125, 80);
		frmAsdasd.getContentPane().add(btnnbsp);

		JLabel lblnbsp = new JLabel(
				"<html>\u4ECE\u6587\u4EF6\u540D\u4E2D\u63D0\u53D6\u76F8\u5E94\u4FE1\u606F\uFF0C<br>\u5E76\u5C06\u5176\u91CD\u65B0\u6392\u5217\u6210\u4EE5\u4E0B\u683C\u5F0F\uFF1A");
		lblnbsp.setVerticalAlignment(SwingConstants.TOP);
		lblnbsp.setFont(new Font("方正准圆_GBK", Font.PLAIN, 16));
		lblnbsp.setBounds(153, 136, 413, 60);
		frmAsdasd.getContentPane().add(lblnbsp);

		JProgressBar progressBar_Graphic = new JProgressBar();
		progressBar_Graphic.setBounds(153, 262, 300, 14);
		frmAsdasd.getContentPane().add(progressBar_Graphic);

		button_Graphic.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				Pending.setAimPath(txtE_1.getText());
				// graphics.runExe(txtEtooleditorformatfactory.getText());
				button_Revocation.setEnabled(false);
				button_Graphic.setEnabled(false);
				button_Shift.setEnabled(true);
				button_Aim.setEnabled(false);
				button_GraphicExe.setEnabled(false);
				progressBar_Graphic.setValue(100);
			}
		});

		button_Revocation.setEnabled(false);
		button_Revocation.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent arg0) {
				new ThreadRevocation(textAreatextWorkInfo, Pending, progressBar_Standize, btnnbsp, button_Revocation,
						button_Graphic, button_Source,comboBox).start();
			}
		});
		button_Revocation.setBounds(466, 135, 100, 27);
		frmAsdasd.getContentPane().add(button_Revocation);

		JPanel panel = new JPanel();
		panel.setBorder(new MatteBorder(1, 1, 1, 1, (Color) new Color(0, 0, 0)));
		panel.setBackground(Color.LIGHT_GRAY);
		panel.setBounds(9, 131, 562, 90);
		frmAsdasd.getContentPane().add(panel);

		button_Graphic.setBounds(14, 229, 125, 80);
		frmAsdasd.getContentPane().add(button_Graphic);

		JLabel lbljpg = new JLabel("<html>将多种格式的图片转换为jpg格式。");
		lbljpg.setFont(new Font("方正准圆_GBK", Font.PLAIN, 16));
		lbljpg.setVerticalAlignment(SwingConstants.TOP);
		lbljpg.setBounds(153, 229, 413, 20);
		frmAsdasd.getContentPane().add(lbljpg);

		JPanel panel_1 = new JPanel();
		panel_1.setBorder(new MatteBorder(1, 1, 1, 1, (Color) new Color(0, 0, 0)));
		panel_1.setBackground(Color.LIGHT_GRAY);
		panel_1.setBounds(9, 224, 562, 90);
		frmAsdasd.getContentPane().add(panel_1);

		JProgressBar progressBar_Shift1 = new JProgressBar();
		progressBar_Shift1.setBounds(153, 370, 300, 14);
		frmAsdasd.getContentPane().add(progressBar_Shift1);

		JProgressBar progressBar_Shift2 = new JProgressBar();
		progressBar_Shift2.setBounds(153, 389, 300, 14);
		frmAsdasd.getContentPane().add(progressBar_Shift2);

		JSpinner spinner = new JSpinner();
		spinner.setModel(new SpinnerNumberModel(3, 0, 9, 1));
		spinner.setFont(new Font("方正准圆_GBK", Font.PLAIN, 16));
		spinner.setBounds(535, 322, 31, 24);
		frmAsdasd.getContentPane().add(spinner);
		button_Shift.setBounds(14, 323, 125, 80);
		frmAsdasd.getContentPane().add(button_Shift);

		button_Shift.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				new ThreadShift(textAreatextWorkInfo, Pending, progressBar_Shift1, progressBar_Shift2, button_Shift,
						button_restart, spinner).start();
			}
		});

		JLabel lblNewLabel = new JLabel("%阈值：");
		lblNewLabel.setHorizontalAlignment(SwingConstants.RIGHT);
		lblNewLabel.setFont(new Font("方正准圆_GBK", Font.PLAIN, 16));
		lblNewLabel.setBounds(466, 323, 72, 18);
		frmAsdasd.getContentPane().add(lblNewLabel);

		JLabel label_Shift = new JLabel("<html>若新图片体积小于原图片一定百分比，<br>则用新图片替代原图片。注意：该步无法撤销！");
		label_Shift.setFont(new Font("方正准圆_GBK", Font.PLAIN, 16));
		label_Shift.setVerticalAlignment(SwingConstants.TOP);
		label_Shift.setBounds(153, 323, 413, 40);
		frmAsdasd.getContentPane().add(label_Shift);

		button_restart.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent arg0) {
				button_restart.setEnabled(false);
				btnnbsp.setEnabled(true);
				button_GraphicExe.setEnabled(true);
				button_Aim.setEnabled(true);
				button_Source.setEnabled(true);
				spinner.setEnabled(true);
				textAreatextWorkInfo.setText("");
				progressBar_Standize.setValue(0);
				progressBar_Graphic.setValue(0);
				progressBar_Shift1.setValue(0);
				progressBar_Shift2.setValue(0);
				Pending = new PendingFile();
			}
		});

		JPanel panel_3 = new JPanel();
		panel_3.setBorder(new MatteBorder(1, 1, 1, 1, (Color) new Color(0, 0, 0)));
		panel_3.setBackground(Color.LIGHT_GRAY);
		panel_3.setBounds(9, 318, 562, 90);
		frmAsdasd.getContentPane().add(panel_3);
		button_restart.setFont(new Font("方正准圆_GBK", Font.PLAIN, 15));
		button_restart.setEnabled(false);
		button_restart.setBounds(14, 815, 552, 27);
		frmAsdasd.getContentPane().add(button_restart);

		JPanel panel_4 = new JPanel();
		panel_4.setBorder(new MatteBorder(1, 1, 1, 1, (Color) new Color(0, 0, 0)));
		panel_4.setBackground(Color.LIGHT_GRAY);
		panel_4.setBounds(9, 411, 562, 436);
		frmAsdasd.getContentPane().add(panel_4);
	}
}

class ThreadStandize extends Thread {
	private JTextPane area;
	private PendingFile Pending;
	private JProgressBar progressBar;
	private JButton button_Revocation;
	private JButton button_Standize;
	private JButton button_Graphic;
	private JButton button_Source;
	private JButton button_restart;
	private JTextField txtE;
	private JComboBox comboBox;

	public ThreadStandize(JTextPane area, PendingFile pending, JProgressBar progressBar, JButton button_Revocation,
			JButton button_Standize, JButton button_Graphic, JButton button_Source, JButton button_restart,
			JTextField txtE, JComboBox comboBox) {
		super();
		this.area = area;
		Pending = pending;
		this.progressBar = progressBar;
		this.button_Revocation = button_Revocation;
		this.button_Standize = button_Standize;
		this.button_Graphic = button_Graphic;
		this.button_Source = button_Source;
		this.button_restart = button_restart;
		this.txtE = txtE;
		this.comboBox = comboBox;
	}

	public void run() {
		button_Standize.setEnabled(false);
		button_Source.setEnabled(false);
		comboBox.setEnabled(false);
		Pending.setSrcPath(txtE.getText());
		Pending.setStdFmt(comboBox.getSelectedIndex());
		String workInfo;
		while (Pending.getWorkState() != 4) {
			workInfo = Pending.nameStandardization();
			if (workInfo.length() != 0) {
				try {
					if (Pending.getWorkState() == 0 && Pending.getErrCode()[0] != 0) {
						area.getDocument().insertString(area.getDocument().getLength(), workInfo, area.getStyle("red"));
					} else if (Pending.getWorkState() == 2) {
						area.getDocument().insertString(area.getDocument().getLength(), workInfo,
								area.getStyle("blue"));
					} else if (Pending.getWorkState() == 3) {
						if (Pending.getErrCode()[Pending.getNowFldr()] == 0) {
							area.getDocument().insertString(area.getDocument().getLength(), workInfo,
									area.getStyle("magenta"));
						} else {
							area.getDocument().insertString(area.getDocument().getLength(), workInfo,
									area.getStyle("red"));
						}
					} else if (Pending.getWorkState() == 10) {
						area.getDocument().insertString(area.getDocument().getLength(), workInfo,
								area.getStyle("normal"));
					} else if (Pending.getWorkState() == 4) {
						if (Pending.getFldrEfflAmt() != 0) {
							area.getDocument().insertString(area.getDocument().getLength(), workInfo,
									area.getStyle("blue"));
						} else {
							area.getDocument().insertString(area.getDocument().getLength(), workInfo,
									area.getStyle("red"));
						}
					}
					area.setCaretPosition(area.getDocument().getLength());
				} catch (BadLocationException e) {
					e.printStackTrace();
				}
			}
			if (Pending.getFldrEfflAmt() != 0) {
				progressBar.setValue(100 * (Pending.getNowFldr() + 1) / Pending.getFldrEfflAmt());
			} else {
				progressBar.setValue(100);
				button_restart.setEnabled(true);
				if (Pending.getWorkState() == 4) {
					break;
				}
			}
		}
		if (Pending.getWorkState() == 4) {
			button_Revocation.setEnabled(true);
			button_Graphic.setEnabled(true);
		}
	}
}

class ThreadRevocation extends Thread {
	private JTextPane area;
	private PendingFile Pending;
	private JProgressBar progressBar;
	private JButton button_Standize;
	private JButton button_Revocation;
	private JButton button_Graphic;
	private JButton button_Source;
	private JComboBox comboBox;

	public ThreadRevocation(JTextPane area, PendingFile pending, JProgressBar progressBar, JButton button_Standize,
			JButton button_Revocation, JButton button_Graphic, JButton button_Source, JComboBox comboBox) {
		super();
		this.area = area;
		Pending = pending;
		this.progressBar = progressBar;
		this.button_Standize = button_Standize;
		this.button_Revocation = button_Revocation;
		this.button_Graphic = button_Graphic;
		this.button_Source = button_Source;
		this.comboBox = comboBox;
	}

	public void run() {
		button_Revocation.setEnabled(false);
		//button_Graphic.setEnabled(false);
		String workInfo;
		// Pending.setWorkState(11);
		while (Pending.getWorkState() != 0) {
			workInfo = Pending.revocation();
			if (workInfo.length() != 0) {
				try {
					if (Pending.getWorkState() == 4 && Pending.getErrCode()[0] != 0) {
						area.getDocument().insertString(area.getDocument().getLength(), workInfo, area.getStyle("red"));
					} else if (Pending.getWorkState() == 2) {
						area.getDocument().insertString(area.getDocument().getLength(), workInfo,
								area.getStyle("blue"));
					} else if (Pending.getWorkState() == 3) {
						if (Pending.getErrCode()[Pending.getNowFldr()] == 0) {
							area.getDocument().insertString(area.getDocument().getLength(), workInfo,
									area.getStyle("magenta"));
						} else {
							area.getDocument().insertString(area.getDocument().getLength(), workInfo,
									area.getStyle("red"));
						}
					} else if (Pending.getWorkState() == 10) {
						area.getDocument().insertString(area.getDocument().getLength(), workInfo,
								area.getStyle("normal"));
					} else if (Pending.getWorkState() == 0) {
						if (Pending.getFldrEfflAmt() != 0) {
							area.getDocument().insertString(area.getDocument().getLength(), workInfo,
									area.getStyle("blue"));
						} else {
							area.getDocument().insertString(area.getDocument().getLength(), workInfo,
									area.getStyle("red"));
						}
					}
					area.setCaretPosition(area.getDocument().getLength());
				} catch (BadLocationException e) {
					e.printStackTrace();
				}
			}
			progressBar.setValue(100 * (Pending.getNowFldr() + 1) / Pending.getFldrEfflAmt());
		}
		button_Standize.setEnabled(true);
		button_Source.setEnabled(true);
		comboBox.setEnabled(true);
	}
}

class ThreadShift extends Thread {
	private JTextPane area;
	private PendingFile Pending;
	private JProgressBar progressBar1;
	private JProgressBar progressBar2;
	private JButton button_Shift;
	private JButton button_restart;
	private JSpinner spinner;

	public ThreadShift(JTextPane area, PendingFile pending, JProgressBar progressBar1, JProgressBar progressBar2,
			JButton button_Shift, JButton button_restart, JSpinner spinner) {
		super();
		this.area = area;
		Pending = pending;
		this.progressBar1 = progressBar1;
		this.progressBar2 = progressBar2;
		this.button_Shift = button_Shift;
		this.button_restart = button_restart;
		this.spinner = spinner;
	}

	public void run() {
		button_Shift.setEnabled(false);
		spinner.setEnabled(false);
		Pending.setSizeThrshld((int) spinner.getValue());
		String workInfo;
		while (Pending.getWorkState() != 9) {
			workInfo = Pending.fileShift();
			if (workInfo.length() != 0) {
				try {
					if (Pending.getWorkState() == 5 && Pending.getErrCode()[0] != 0) {
						area.getDocument().insertString(area.getDocument().getLength(), workInfo, area.getStyle("red"));
					} else if (Pending.getWorkState() == 7) {
						area.getDocument().insertString(area.getDocument().getLength(), workInfo,
								area.getStyle("blue"));
					} else if (Pending.getWorkState() == 8) {
						if (Pending.getErrCode()[Pending.getNowFldr()] == 0) {
							area.getDocument().insertString(area.getDocument().getLength(), workInfo,
									area.getStyle("magenta"));
						} else {
							area.getDocument().insertString(area.getDocument().getLength(), workInfo,
									area.getStyle("red"));
						}
					} else if (Pending.getWorkState() == 10) {
						area.getDocument().insertString(area.getDocument().getLength(), workInfo,
								area.getStyle("normal"));
					} else if (Pending.getWorkState() == 9) {
						if (Pending.getFldrEfflAmt() != 0) {
							area.getDocument().insertString(area.getDocument().getLength(), workInfo,
									area.getStyle("blue"));
						} else {
							area.getDocument().insertString(area.getDocument().getLength(), workInfo,
									area.getStyle("red"));
						}
					}
					area.setCaretPosition(area.getDocument().getLength());
				} catch (BadLocationException e) {
					e.printStackTrace();
				}
			}
			if (Pending.getErrCode()[0] != 10) {
				progressBar1.setValue(100 * (Pending.getNowFldr()) / Pending.getFldrEfflAmt());
				if (Pending.getWorkState() == 9) {
					Pending.setNowFldr(Pending.getNowFldr() - 1);
				}
				progressBar2.setValue(
						100 * (Pending.getNowFile() + 1) / Pending.getFileEfflAmtPerFolder()[Pending.getNowFldr()]);
			} else {
				progressBar1.setValue(100);
				progressBar2.setValue(100);
				break;
			}
		}
		button_restart.setEnabled(true);
	}
}
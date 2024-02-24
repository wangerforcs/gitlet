package gitlet;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;


public class Main{
    public static void main(String[] args){
        String exePath = "/home/wnger/workplace/gitprj/gitlet/gitlet";
        // 直接相加变字符串会导致commit的message出问题
        String[] cmd = new String[args.length + 1];
        cmd[0] = exePath;
        for (int i = 0; i < args.length; i++){
            cmd[i+1] = args[i];
        }
        openExe(cmd);
    }
    public static void openExe(String[] cmd) {
        BufferedReader br = null;
        BufferedReader brError = null;
 
        try {
            //执行exe  cmd可以为字符串(exe存放路径)也可为数组，调用exe时需要传入参数时，可以传数组调用(参数有顺序要求)
            Process p = Runtime.getRuntime().exec(cmd);
            String line = null;
            //获得子进程的输入流。
            br = new BufferedReader(new InputStreamReader(p.getInputStream()));
            //获得子进程的错误流。
            brError = new BufferedReader(new InputStreamReader(p.getErrorStream()));
            while ((line = br.readLine()) != null  || (line = brError.readLine()) != null) {
                //输出exe输出的信息以及错误信息
                System.out.println(line);
            }
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            if (br != null) {
                try {
                    br.close();
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }
    }

}
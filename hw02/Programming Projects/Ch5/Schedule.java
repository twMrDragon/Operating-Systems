/*
    According to original book.
    The priority is higher-priority.
    Assume all tasks came from same time.
    RR quantum is 10.


    Parameter:
        FCFS
        SJF
        RR
        Priority
        PriWRR
 */



import java.util.*;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

class Triple{
    String task;
    int time;
    int priority;
    Triple(String task, int time, int priority){
        this.task = task;
        this.time = time;
        this.priority = priority;
    }
}


public class Schedule {
    private static void FCFS(Vector<Triple> a){
        int t = 0;
        int n = a.size();
        System.out.println("Starting Execute by FCFS:");
        String ans = "";
        for(int i = 0; i < n; i++){
            ans+=a.get(i).task;
            if(i!=n-1){
                ans+=",";
            }
            t += a.get(i).time;
        }
        System.out.println("Sequence is:"+ans);
        System.out.println("Total time is:"+t);
    }
    private static void SJF(Vector<Triple> a){
        int t = 0;
        int n = a.size();
        PriorityQueue<Triple> queue = new PriorityQueue<>(
                Comparator.comparingInt((Triple x) -> x.time) // 先根據 time 排序 (SJF)
                        .thenComparingInt(x -> Integer.parseInt(x.task.replaceAll("[^0-9]", ""))) // 再根據 task 中的數字部分排序
        );

        for(int i = 0; i < n; i++){
            queue.add(a.get(i));
        }


        System.out.println("Starting Execute by SJF:");
        String ans = "";
        for(int i = 0; i < n; i++){
            Triple tmp = queue.poll();
            ans+=tmp.task;
            if(i!=n-1){
                ans+=",";
            }
            t += tmp.time;
        }
        System.out.println("Sequence is:"+ans);
        System.out.println("Total time is:"+t);
    }
    private static void Priority(Vector<Triple> a){
        int t = 0;
        int n = a.size();
        PriorityQueue<Triple> queue = new PriorityQueue<>(
                (x, y) -> {
                    // 先比較優先級，較大的優先級排在前面
                    int priorityCompare = Integer.compare(y.priority, x.priority);
                    if (priorityCompare != 0) {
                        return priorityCompare;  // 如果優先級不同，直接返回結果
                    }
                    // 如果優先級相同，則比較 time，較短的 time 排在前面（SJF）
                    return Integer.compare(x.time, y.time);  // 這裡是 SJF 比較，時間較短的優先
                }
        );
        for(int i = 0; i < n; i++){
            queue.add(a.get(i));
        }


        System.out.println("Starting Execute by Priority:");
        String ans = "";
        for(int i = 0; i < n; i++){
            Triple tmp = queue.poll();
            ans+=tmp.task;
            if(i!=n-1){
                ans+=",";
            }
            t += tmp.time;
        }
        System.out.println("Sequence is:"+ans);
        System.out.println("Total time is:"+t);
    }
    private static void RR(Vector<Triple> a){
        Queue<Triple> queue = new LinkedList<>();
        for(int i = 0; i < a.size(); i++){
            queue.add(new Triple(a.get(i).task, a.get(i).time, a.get(i).priority));
        }
        int t = 0;

        System.out.println("Starting Execute by RR:");
        String ans = "";
        while (!queue.isEmpty()){
            Triple tmp = queue.poll();
            if(tmp.time <= 0){
                continue;
            }
            ans+=tmp.task;
            t += Math.min(tmp.time, 10);
            tmp.time -= 10;
            ans+=",";
            queue.add(tmp);
        }
        ans = ans.substring(0, ans.length() - 1);
        System.out.println("Sequence is:"+ans);
        System.out.println("Total time is:"+t);
    }
    private static void PriorityWithRR(Vector<Triple> a){

        LinkedList<Triple>[] queue = new LinkedList[11];

        for (int i = 0; i <= 10; i++) {
            queue[i] = new LinkedList<>();
        }

        for(int i = 0; i < a.size(); i++){
            queue[a.get(i).priority].add(new Triple(a.get(i).task, a.get(i).time, a.get(i).priority));
        }
        int t = 0;

        System.out.println("Starting Execute by Priority With RR:");
        String ans = "";

        for(int i = queue.length - 1; i >= 0; i--){
            while (!queue[i].isEmpty()){
                Triple tmp = queue[i].poll();
                t+=Math.min(tmp.time, 10);
                tmp.time -= 10;
                ans += tmp.task;
                ans+=",";
                if(tmp.time<=0){
                    continue;
                }
                queue[i].add(tmp);
            }
        }
        ans = ans.substring(0, ans.length() - 1);
        System.out.println("Sequence is:"+ans);
        System.out.println("Total time is:"+t);
    }
    public static void main(String[] args) {
        if (args.length < 1) {
            System.out.println("Usage: java Schedule <algorithm>");
            return;
        }

        Vector<Triple> a = new Vector<>();
        String filePath = "example.txt";
        try (BufferedReader reader = new BufferedReader(new FileReader(filePath))) {
            String line;
            while ((line = reader.readLine()) != null) {
                // 用空白分割每一行
                String[] parts = line.trim().split("\\s+");

                if (parts.length == 3) {
                    String name = parts[0];
                    int number1 = Integer.parseInt(parts[1]);
                    int number2 = Integer.parseInt(parts[2]);

                    a.add(new Triple(name, number1, number2));
                } else {
                    System.out.println("Format Error：" + line);
                    System.out.println("Format Should be <PX> <Time> <Priority>");
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }

        switch (args[0]) {
            case "FCFS":
                FCFS(a);
                break;
            case "SJF":
                SJF(a);
                break;
            case "Priority":
                Priority(a);
                break;
            case "RR":
                RR(a);
                break;
            case "PriWRR":
                PriorityWithRR(a);
                break;
            case "All":
                FCFS(a);
                SJF(a);
                RR(a);
                Priority(a);
                PriorityWithRR(a);
                break;
            default:
                System.out.println("Argument Error! Should be FCFS or SJF or Priority or RR or PriWRR or All");
                break;
        }

    }
}

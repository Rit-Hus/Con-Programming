

import java.net.*;
import java.io.*;

public class Teacher {
    static int PORT = 5010;

    public static void main(String[] args) {
        // Handels worng input when compiling
        if (args.length < 1) {
            System.out.println("Usage: java Teacher <number_of_students>");
            return;
        }

        int totalStudents = Integer.parseInt(args[0]);
        int studentsProcessed = 0;
        boolean isOddCount = (totalStudents % 2 == 1);

        try (ServerSocket teacherSocket = new ServerSocket(PORT)) {
            System.out.println("Teacher: Waiting for students to submit requests...");

            while (studentsProcessed < totalStudents) {
                // Student 1
                Socket student1 = teacherSocket.accept();
                studentsProcessed++;
                int id1 = studentsProcessed;

                // Odd student
                if (isOddCount && studentsProcessed == totalStudents) {
                    PrintWriter out1 = new PrintWriter(student1.getOutputStream(), true);
                    out1.println(id1); // Works alone
                    student1.close();
                } else {
                    // Student 2
                    Socket student2 = teacherSocket.accept();
                    studentsProcessed++;
                    int id2 = studentsProcessed;

                    // Tell Student 1 about the other and vice versa
                    PrintWriter out1 = new PrintWriter(student1.getOutputStream(), true);
                    PrintWriter out2 = new PrintWriter(student2.getOutputStream(), true);

                    out1.println(id2); 
                    out2.println(id1);

                    System.out.println("Teacher: Formed group with Student " + id1 + " and Student " + id2);

                    student1.close();
                    student2.close();
                }
            }
            System.out.println("Teacher: All students paired.");
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}

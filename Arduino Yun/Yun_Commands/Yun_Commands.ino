/*
 * File: Yun_Commands.ino
 * Running process using Process class. 
 * This sketch demonstrate how to run linux processes using an Arduino Yún. 
 * 
 * created 5 Jun 2013 by Cristian Maglie
 * Modifications by Claus Kühnel 2020-02-14
 * 
 * This example code is in the public do-main.
 */
#include <Process.h>
#include <Console.h>

void setup() 
{
  Bridge.begin(); // Initialize Bridge
  Console.begin(); // Initialize Console

  // Wait until a Console is connected.
  while (!Console);

  // Run different Linux commands
  runCpuInfo();
  runUptime();
}

void loop() {}


void runCpuInfo() 
{
  // Launch "cat /proc/cpuinfo" command (shows info on Atheros CPU)
  // cat is a command line utility that shows the content of a file
  Process p;        // Create a process and call it "p"
  p.begin("cat");   // Process that launch the "cat" command
  p.addParameter("/proc/cpuinfo"); // Add the cpuinfo file path as parameter to cat
  p.run();      // Run the process and wait for its termination

  Console.println("\nCPU Info:");
  
  // Print command output on the Serial.
  // A process output can be read with the stream methods
  while (p.available()>0) 
  {
    char c = p.read();
    Console.print(c);
  }
  // Ensure the last bit of data is sent.
  Console.flush();
}

void runUptime() 
{
  Process p;        // Create a process and call it "p"
  p.runShellCommand("uptime"); // Add the cpuinfo file path as parameter to cut
  p.run();      // Run the process and wait for its termination

  Console.println("Uptime:");
  
  // Print command output on the Serial.
  // A process output can be read with the stream methods
  while (p.available()>0) 
  {
    char c = p.read();
    Console.print(c);
  }
  // Ensure the last bit of data is sent.
  Console.flush();
}

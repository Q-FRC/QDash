// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package frc.robot;

import java.util.LinkedList;
import java.util.Queue;

import edu.wpi.first.networktables.NetworkTable;
import edu.wpi.first.networktables.NetworkTableEntry;
import edu.wpi.first.networktables.NetworkTableInstance;

/** Class for interacting with QFRCDashboard. */
public class QFRCLib {
    public enum ErrorLevel {
        Information,
        Warning,
        Critical
    }

    private static class Error {
        private final String m_message;
        private final ErrorLevel m_level;

        public Error(String message, ErrorLevel level) {
            m_message = message;
            m_level = level;
        }

        public String getMessage() {
            return m_message;
        }

        public ErrorLevel getLevel() {
            return m_level;
        }
    }

    private static final NetworkTableInstance inst = NetworkTableInstance.getDefault();
    private static final NetworkTable table = inst.getTable("QFRCDashboard");

    private static final NetworkTableEntry tabEntry = table.getEntry("Tab");
    private static final NetworkTableEntry errorsEntry = table.getEntry("Errors");

    private static final Queue<Error> errors = new LinkedList<>();
    private static int errorHistoryLength = 5;

    /**
     * Reports an error or info to be displayed on the dashboard.
     * @param level The error level (info, warning, critical)
     * @param message The error message.
     */
    public static void reportError(ErrorLevel level, String message) {
        Error e = new Error(message, level);
        errors.offer(e);
        if (errors.size() > errorHistoryLength) {
            errors.poll();
        }

        publishErrors();
    }

    private static void publishErrors() {
        String[] array = new String[errors.size() * 2];
        int i = 0;
        for (Error e : errors) {
            array[i] = e.getLevel().toString();
            ++i;
            array[i] = e.getMessage().toString();
            ++i;
        }
        errorsEntry.setStringArray(array);
    }

    /**
     * Sets the length of the error queue.
     * @param length The length of the error queue.
     */
    public static void setErrorHistoryLength(int length) {
        errorHistoryLength = length;
        while (errors.size() > errorHistoryLength) {
            errors.poll();
        }
    }

    /**
     * Sets the dashboard to the specified tab.
     * @param tabName The tab to switch to.
     * @apiNote This is a no-op if the specified tab does not exist.
     */
    public static void setTab(String tabName) {
        tabEntry.setString(tabName);
    }
}

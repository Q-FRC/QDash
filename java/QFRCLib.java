// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package frc.robot;

import java.util.LinkedList;
import java.util.Queue;

import com.fasterxml.jackson.annotation.JsonProperty;
import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;

import edu.wpi.first.net.WebServer;
import edu.wpi.first.networktables.NetworkTable;
import edu.wpi.first.networktables.NetworkTableEntry;
import edu.wpi.first.networktables.NetworkTableInstance;
import edu.wpi.first.networktables.PubSubOption;
import edu.wpi.first.networktables.StringPublisher;
import edu.wpi.first.networktables.StringTopic;
import edu.wpi.first.wpilibj.Filesystem;

/** Class for interacting with QDash. */
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
    private static final NetworkTable table = inst.getTable("QDash");

    private static final NetworkTableEntry tabEntry = table.getEntry("Tab");
    private static final NetworkTableEntry errorsEntry = table.getEntry("Errors");

    private static final Queue<Error> errors = new LinkedList<>();
    private static int errorHistoryLength = 5;

    /**
     * Reports an error or info to be displayed on the dashboard.
     *
     * @param level   The error level (info, warning, critical)
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
     *
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
     *
     * @param tabName The tab to switch to.
     * @apiNote This is a no-op if the specified tab does not exist.
     */
    public static void setTab(String tabName) {
        tabEntry.setString(tabName);
    }

    /**
     * Start the web server for accessing remote layouts.
     */
    public static void startWebServer() {
        WebServer.start(5800, Filesystem.getDeployDirectory().getPath());
    }

    /* NOTIFICATIONS */
    // Modified from ElasticLib's notification API.
    // Thanks to Gold872 and others.

    private static final StringTopic topic = NetworkTableInstance.getDefault()
            .getStringTopic("/QDash/RobotNotifications");
    private static final StringPublisher publisher = topic.publish(PubSubOption.sendAll(true),
            PubSubOption.keepDuplicates(true));
    private static final ObjectMapper objectMapper = new ObjectMapper();

    /**
     * Sends an notification to the Elastic dashboard. The notification is
     * serialized as a JSON string
     * before being published.
     *
     * @param notification the {@link Notification} object containing notification
     *                     details
     */
    public static void sendNotification(Notification notification) {
        try {
            publisher.set(objectMapper.writeValueAsString(notification));
        } catch (JsonProcessingException e) {
            e.printStackTrace();
        }
    }

    /**
     * Represents an notification object to be sent to the Elastic dashboard. This
     * object holds
     * properties such as level, title, description, display time, and dimensions to
     * control how the
     * notification is displayed on the dashboard.
     */
    public static class Notification {
        @JsonProperty("level")
        private ErrorLevel level;

        @JsonProperty("title")
        private String title;

        @JsonProperty("description")
        private String description;

        @JsonProperty("displayTime")
        private int displayTimeMillis;

        @JsonProperty("width")
        private double width;

        @JsonProperty("height")
        private double height;

        /**
         * Creates a new Notification with all default parameters. This constructor is
         * intended
         * to be used with the chainable decorator methods.
         *
         * <p>
         * Title and description fields are empty.
         */
        public Notification() {
            this(ErrorLevel.Information, "", "");
        }

        /**
         * Creates a new Notification with all properties specified.
         *
         * @param level             the level of the notification (Information, Warning,
         *                          Critical)
         * @param title             the title text of the notification
         * @param description       the descriptive text of the notification
         * @param displayTimeMillis the time in milliseconds for which the notification
         *                          is displayed
         * @param width             the width of the notification display area
         * @param height            the height of the notification display area,
         *                          inferred if below zero
         */
        public Notification(
                ErrorLevel level,
                String title,
                String description,
                int displayTimeMillis,
                double width,
                double height) {
            this.level = level;
            this.title = title;
            this.displayTimeMillis = displayTimeMillis;
            this.description = description;
            this.height = height;
            this.width = width;
        }

        /**
         * Creates a new Notification with default display time and dimensions.
         *
         * @param level       the level of the notification
         * @param title       the title text of the notification
         * @param description the descriptive text of the notification
         */
        public Notification(ErrorLevel level, String title, String description) {
            this(level, title, description, 3000, 350, -1);
        }

        /**
         * Creates a new Notification with a specified display time and default
         * dimensions.
         *
         * @param level             the level of the notification
         * @param title             the title text of the notification
         * @param description       the descriptive text of the notification
         * @param displayTimeMillis the display time in milliseconds
         */
        public Notification(
                ErrorLevel level, String title, String description, int displayTimeMillis) {
            this(level, title, description, displayTimeMillis, 350, -1);
        }

        /**
         * Creates a new Notification with specified dimensions and default display
         * time. If the height
         * is below zero, it is automatically inferred based on screen size.
         *
         * @param level       the level of the notification
         * @param title       the title text of the notification
         * @param description the descriptive text of the notification
         * @param width       the width of the notification display area
         * @param height      the height of the notification display area, inferred if
         *                    below zero
         */
        public Notification(
                ErrorLevel level, String title, String description, double width, double height) {
            this(level, title, description, 3000, width, height);
        }

        /**
         * Updates the level of this notification
         *
         * @param level the level to set the notification to
         */
        public void setLevel(ErrorLevel level) {
            this.level = level;
        }

        /**
         * @return the level of this notification
         */
        public ErrorLevel getLevel() {
            return level;
        }

        /**
         * Updates the title of this notification
         *
         * @param title the title to set the notification to
         */
        public void setTitle(String title) {
            this.title = title;
        }

        /**
         * Gets the title of this notification
         *
         * @return the title of this notification
         */
        public String getTitle() {
            return title;
        }

        /**
         * Updates the description of this notification
         *
         * @param description the description to set the notification to
         */
        public void setDescription(String description) {
            this.description = description;
        }

        public String getDescription() {
            return description;
        }

        /**
         * Updates the display time of the notification
         *
         * @param seconds the number of seconds to display the notification for
         */
        public void setDisplayTimeSeconds(double seconds) {
            setDisplayTimeMillis((int) Math.round(seconds * 1000));
        }

        /**
         * Updates the display time of the notification in milliseconds
         *
         * @param displayTimeMillis the number of milliseconds to display the
         *                          notification for
         */
        public void setDisplayTimeMillis(int displayTimeMillis) {
            this.displayTimeMillis = displayTimeMillis;
        }

        /**
         * Gets the display time of the notification in milliseconds
         *
         * @return the number of milliseconds the notification is displayed for
         */
        public int getDisplayTimeMillis() {
            return displayTimeMillis;
        }

        /**
         * Updates the width of the notification
         *
         * @param width the width to set the notification to
         */
        public void setWidth(double width) {
            this.width = width;
        }

        /**
         * Gets the width of the notification
         *
         * @return the width of the notification
         */
        public double getWidth() {
            return width;
        }

        /**
         * Updates the height of the notification
         *
         * <p>
         * If the height is set to -1, the height will be determined automatically by
         * the dashboard
         *
         * @param height the height to set the notification to
         */
        public void setHeight(double height) {
            this.height = height;
        }

        /**
         * Gets the height of the notification
         *
         * @return the height of the notification
         */
        public double getHeight() {
            return height;
        }

        /**
         * Modifies the notification's level and returns itself to allow for method
         * chaining
         *
         * @param level the level to set the notification to
         * @return the current notification
         */
        public Notification withLevel(ErrorLevel level) {
            this.level = level;
            return this;
        }

        /**
         * Modifies the notification's title and returns itself to allow for method
         * chaining
         *
         * @param title the title to set the notification to
         * @return the current notification
         */
        public Notification withTitle(String title) {
            setTitle(title);
            return this;
        }

        /**
         * Modifies the notification's description and returns itself to allow for
         * method chaining
         *
         * @param description the description to set the notification to
         * @return the current notification
         */
        public Notification withDescription(String description) {
            setDescription(description);
            return this;
        }

        /**
         * Modifies the notification's display time and returns itself to allow for
         * method chaining
         *
         * @param seconds the number of seconds to display the notification for
         * @return the current notification
         */
        public Notification withDisplaySeconds(double seconds) {
            return withDisplayMilliseconds((int) Math.round(seconds * 1000));
        }

        /**
         * Modifies the notification's display time and returns itself to allow for
         * method chaining
         *
         * @param displayTimeMillis the number of milliseconds to display the
         *                          notification for
         * @return the current notification
         */
        public Notification withDisplayMilliseconds(int displayTimeMillis) {
            setDisplayTimeMillis(displayTimeMillis);
            return this;
        }

        /**
         * Modifies the notification's width and returns itself to allow for method
         * chaining
         *
         * @param width the width to set the notification to
         * @return the current notification
         */
        public Notification withWidth(double width) {
            setWidth(width);
            return this;
        }

        /**
         * Modifies the notification's height and returns itself to allow for method
         * chaining
         *
         * @param height the height to set the notification to
         * @return the current notification
         */
        public Notification withHeight(double height) {
            setHeight(height);
            return this;
        }

        /**
         * Modifies the notification's height and returns itself to allow for method
         * chaining
         *
         * <p>
         * This will set the height to -1 to have it automatically determined by the
         * dashboard
         *
         * @return the current notification
         */
        public Notification withAutomaticHeight() {
            setHeight(-1);
            return this;
        }
    }
}

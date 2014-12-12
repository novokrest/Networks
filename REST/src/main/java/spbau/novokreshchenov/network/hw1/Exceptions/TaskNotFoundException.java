package spbau.novokreshchenov.network.hw1.Exceptions;

import com.sun.jersey.api.MessageException;


public class TaskNotFoundException extends MessageException {
    private final String taskId;

    public TaskNotFoundException(String taskId) {
        super("Task with specified id=\'" + taskId + "\' doesn't exist");
        this.taskId = taskId;
    }

    public String getTaskId() {
        return taskId;
    }
}

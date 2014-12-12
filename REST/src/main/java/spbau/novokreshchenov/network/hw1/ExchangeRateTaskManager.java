package spbau.novokreshchenov.network.hw1;


import com.sun.jmx.snmp.tasks.Task;
import spbau.novokreshchenov.network.hw1.Exceptions.BankNotFoundException;
import spbau.novokreshchenov.network.hw1.Exceptions.ExchangeRateNotFoundException;
import spbau.novokreshchenov.network.hw1.Exceptions.TaskNotFoundException;

import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.*;

public class ExchangeRateTaskManager {
    private static final int THREADS_COUNT = 10;
    private final Map<String, FutureTask<ExchangeRateTaskResult>> tasks = new HashMap<String, FutureTask<ExchangeRateTaskResult>>();
    private int taskCounter = 0;

    private final ExchangeRateSearcher searcher;
    private final ExecutorService executor;

    public ExchangeRateTaskManager(ExchangeRateSearcher searcher) {
        executor = Executors.newFixedThreadPool(THREADS_COUNT);
        this.searcher = searcher;
    }

    public String putTask(final ExchangeRateTask task) {
        String taskId = Integer.toString(taskCounter++);

        FutureTask<ExchangeRateTaskResult> futureTask = new FutureTask<ExchangeRateTaskResult>(
                new Callable<ExchangeRateTaskResult>() {
                    @Override
                    public ExchangeRateTaskResult call() throws BankNotFoundException, ExchangeRateNotFoundException {
                        ExchangeRate exchangeRate = searcher.getExchangeRate(task.getBankName(), task.getCurrency());
                        return new ExchangeRateTaskResult(task, exchangeRate.getBuyValue(), exchangeRate.getSellValue());
                    }
                }
        );

        tasks.put(taskId, futureTask);
        executor.execute(futureTask);

        return taskId;
    }

    public void postTask(String taskId, final ExchangeRateTask newTask) throws TaskNotFoundException {
        if (!tasks.containsKey(taskId)) {
            throw new TaskNotFoundException(taskId);
        }

        FutureTask<ExchangeRateTaskResult> oldFutureTask = tasks.remove(taskId);
        oldFutureTask.cancel(true);
        FutureTask<ExchangeRateTaskResult> updatedFutureTask = new FutureTask<ExchangeRateTaskResult>(
                new Callable<ExchangeRateTaskResult>() {
                    @Override
                    public ExchangeRateTaskResult call() throws BankNotFoundException, ExchangeRateNotFoundException {
                        ExchangeRate exchangeRate = searcher.getExchangeRate(newTask.getBankName(), newTask.getCurrency());
                        return new ExchangeRateTaskResult(newTask, exchangeRate.getBuyValue(), exchangeRate.getSellValue());
                    }
                }
        );

        executor.execute(updatedFutureTask);
    }

    public void deleteTask(String taskId) throws TaskNotFoundException {
        if (!tasks.containsKey(taskId)) {
            throw new TaskNotFoundException(taskId);
        }

        tasks.remove(taskId).cancel(true);
    }

    public ExchangeRateTaskResult getTaskResult(String taskId) throws BankNotFoundException, ExchangeRateNotFoundException {
        if (!tasks.containsKey(taskId)) {
            throw new TaskNotFoundException(taskId);
        }

        ExchangeRateTaskResult result = null;
        try {
            result = tasks.get(taskId).get();
        } catch (InterruptedException e) {
        } catch (ExecutionException e) {
            Throwable t = e.getCause();
            if (t instanceof BankNotFoundException) {
                throw (BankNotFoundException) t;
            }
            if (t instanceof ExchangeRateNotFoundException) {
                throw (ExchangeRateNotFoundException) t;
            }
        }

        return result;
    }
}

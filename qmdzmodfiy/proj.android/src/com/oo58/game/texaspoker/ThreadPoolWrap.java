package com.oo58.game.texaspoker;



import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

public class ThreadPoolWrap {

	private static final int DEFAULT_COREPOOLSIZE = 2;
	private static final long DEFAULT_KEEP_ALIVE_TIME = 30L;
	private static final int DEFAULT_MAXIMUM_POOLSIZE = 20;
	private static ThreadPoolWrap instance;
	private BlockingQueue<Runnable> bq;
	private ThreadPoolExecutor executor ;

	private ThreadPoolWrap() {
		executor = null;
		bq = new ArrayBlockingQueue(50);
		executor = new ThreadPoolExecutor(DEFAULT_COREPOOLSIZE, DEFAULT_MAXIMUM_POOLSIZE, DEFAULT_KEEP_ALIVE_TIME, TimeUnit.SECONDS, bq);
	}

	public static ThreadPoolWrap getThreadPool() {
		if (instance == null)
			instance = new ThreadPoolWrap();
		return instance;
	}

	public void executeTask(Runnable paramRunnable) {
		this.executor.execute(paramRunnable);
	}

	public boolean isThreadPoolActive() {
		boolean flag = true;
		if (executor.getActiveCount() < 1)
			flag = false;
		return flag;
	}

	public void removeTask(Runnable paramRunnable) {
		this.executor.remove(paramRunnable);
	}

	public void shutdown() {
		this.executor.shutdown();
		instance = null;
	}
}

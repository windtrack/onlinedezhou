package tv.danmaku.ijk.media.widget;

import android.view.KeyEvent;
import android.view.MotionEvent;

public interface OptionDelegate {
	public abstract void onTouchEvent1(MotionEvent ev);
	public abstract boolean onTrackballEvent1(MotionEvent ev);
	public abstract boolean onKeyDown1(int keyCode, KeyEvent event);
}

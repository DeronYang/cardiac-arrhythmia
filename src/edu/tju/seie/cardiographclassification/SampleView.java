package edu.tju.seie.cardiographclassification;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Paint.Style;
import android.graphics.Path;
import android.util.AttributeSet;
import android.util.Log;
import android.view.View;

public class SampleView extends View {
	private int mHeight = 440;
	private int mWidth = 1280;
	private float[] pointArray;
	float fStartX = 40.0f;
	float fEndX = 1240.0f;
	float fStartY = 20.0f;
	float fEndY = 420.0f;
	float fzeroline = 220.0f;
	private Paint mPaint;

	public SampleView(Context context, AttributeSet attrs) {
		super(context, attrs);
		// TODO Auto-generated constructor stub
		mPaint = new Paint();
	}

	public SampleView(Context context) {
		super(context);
		// TODO Auto-generated constructor stub
		mPaint = new Paint();
	}

	public void setPointArray(float[] array) {
		this.pointArray = array;
	}

	@Override
	protected void onDraw(Canvas canvas) {
		// TODO Auto-generated method stub
		super.onDraw(canvas);

		fStartX = 40;
		fEndX = 1140;
		canvas.drawColor(Color.WHITE);// Çå³ý±³¾°
		// TODO: handle exception

		mPaint.setStrokeWidth(1);
		mPaint.setStyle(Style.STROKE);
		mPaint.setColor(Color.argb(255, 248, 195, 175));
		for (int yy = 0; yy < 21; yy++) {
			canvas.drawLine(fStartX, fStartY + 20 * yy, fEndX, fStartY + 20
					* yy, mPaint);
		}
		for (int xx = 0; xx < 56; xx++) {
			canvas.drawLine(fStartX + xx * 20, fStartY, fStartX + xx * 20,
					fEndY, mPaint);
		}

		mPaint.setColor(Color.argb(255, 240, 152, 116));
		for (int yy = 0; yy < 6; yy++) {
			canvas.drawLine(fStartX, fStartY + 100 * yy, fEndX, fStartY + 100
					* yy, mPaint);
		}
		for (int xx = 0; xx < 12; xx++) {
			canvas.drawLine(fStartX + xx * 100, fStartY, fStartX + xx * 100,
					fEndY, mPaint);
		}

		if (pointArray == null) {
			return;
		}
		mPaint.setColor(Color.BLACK);
		mPaint.setStrokeWidth(1);
		mPaint.setStyle(Style.STROKE);
		Path path = new Path();
		float x = 40.0f;
		float yscale = 0.6f;
		path.moveTo(x, yscale * (1024 - pointArray[0]) + fzeroline);
		x = x + 0.8f;
		for(int i = 1; i < pointArray.length; i++){
			path.lineTo(x,yscale * (1024 - pointArray[i]) + fzeroline);
			x = x + 0.8f;
		}
		canvas.drawPath(path, mPaint);
	}

}

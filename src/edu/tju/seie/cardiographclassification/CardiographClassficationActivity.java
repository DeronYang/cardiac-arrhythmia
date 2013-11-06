package edu.tju.seie.cardiographclassification;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;

import android.app.Activity;
import android.os.Bundle;
import android.os.Environment;
import android.text.InputType;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

public class CardiographClassficationActivity extends Activity {
	/** Called when the activity is first created. */
	private static final String modelFile = "/mnt/sdcard/ECG/model.dat";
	private static final String TAG = "Libsvm";
	private float[] pointArray;
	private int[] groundtruthArray;
	private SampleView sampleView;
	private int group = 1;
	private int drawfactor = 1300;
	private int drawshift  = 50;
	EditText et;
	TextView tv;
	int[] labels;
	double[] probs;
	// svm native
	private native int doClassificationNative(int beg,
			int end, int labels[],double probs[],String modelFile);

	// Load the native library
	static {
		try {
			System.loadLibrary("signal");
		} catch (UnsatisfiedLinkError ule) {
			Log.e(TAG, "Hey, could not load native library signal");
		}
	}

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);
		sampleView = (SampleView)findViewById(R.id.sampleview);
		

		Button lastButton = (Button) findViewById(R.id.last);
		Button nextButton = (Button) findViewById(R.id.next);
		Button goButton = (Button) findViewById(R.id.go);
		et = (EditText) findViewById(R.id.edittext);
		et.setHint("Input Group Number: 1-519");
		et.setInputType(InputType.TYPE_CLASS_NUMBER);
		tv = (TextView)findViewById(R.id.tv);
		lastButton.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				Log.v("TAG","last");
				group = group - 1;
				if(group<=1){
					group = 1;
					Toast.makeText(getApplicationContext(), "No more", Toast.LENGTH_SHORT).show();
					Log.v("TAG","small");
				}
				et.setText(group+"");
				readSample(1+drawfactor*group+drawshift,drawfactor*(group+1)+drawshift);
				sampleView.setPointArray(pointArray);
				sampleView.invalidate();
				classify(1+5*(group-1),group*5);
			}
		});
		nextButton.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				Log.v("TAG","next");
				group = group + 1;
				if(group>=519){
					group = 519;
					Toast.makeText(getApplicationContext(), "No more", Toast.LENGTH_SHORT).show();
					Log.v("TAG","big");
				}
				et.setText(group+"");
				readSample(1+drawfactor*group+drawshift,drawfactor*(group+1)+drawshift);
				sampleView.setPointArray(pointArray);
				sampleView.invalidate();
				classify(1+5*(group-1),group*5);
			}
		});
		goButton.setOnClickListener(new View.OnClickListener() {
			
			public void onClick(View v) {
				// TODO Auto-generated method stub
				group = Integer.parseInt(et.getEditableText().toString());
				if(group>519)
					group = 519;
				if(group<1)
					group = 1;
				readSample(1+drawfactor*group+drawshift,drawfactor*(group+1)+drawshift);
				sampleView.setPointArray(pointArray);
				sampleView.invalidate();
				classify(1+5*(group-1),group*5);
			}
		});
	}

	private void classify(int beg, int end){
		labels = new int[end-beg+1];
		probs = new double[end-beg+1];
		doClassificationNative(beg, end, labels, probs, modelFile);
		StringBuilder sb = new StringBuilder();
		sb.append("result            : ");
		for(int i = 0; i < labels.length; i++){
			sb.append(labels[i]);
			sb.append(" ");
		}
		sb.append("\n");
		sb.append("ground truth : ");
		readGroundTruth(beg, end);
		for(int i = 0; i < labels.length; i++){
			sb.append(groundtruthArray[i]);
			sb.append(" ");
		}
		sb.append("\n");
		sb.append("1:arrhythmia\n");
		sb.append("2:normal");
		tv.setText(sb.toString());
		
		
	}
	private void readSample(int beg, int end) {
		BufferedReader reader = null;
		if (beg < 1 || end < 1 || end < beg)
			return;
		pointArray = new float[end - beg + 1];
		try {

			FileReader fr = new FileReader("mnt/sdcard/ECG/samp_pre105.log");
			reader = new BufferedReader(fr);
			int i = 1;
			while (i < beg) {
				reader.readLine();
				i++;
			}
			i = 0;
			String tmp;
			// String[][] ps = new String[end-beg+1][2];
			while (i < end - beg + 1 && (tmp = reader.readLine()) != null) {
				// ps[i][0]=tmp.split("\\t")[0];
				// ps[i][1]=tmp.split("\\t")[1];
				// pointArray[i] = new PointF();
				// pointArray[i].x = Float.parseFloat(ps[i][0]);
				// pointArray[i].y = Float.parseFloat(ps[i][1]);
				// pointArray[i].y = pointArray[i].y / 2.4f / 1000;
				pointArray[i] = Float.parseFloat(tmp);
				i++;
			}

		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

	}
	private void readGroundTruth(int beg, int end) {
		BufferedReader reader = null;
		if (beg < 1 || end < 1 || end < beg)
			return;
		groundtruthArray = new int[end - beg + 1];
		try {

			FileReader fr = new FileReader("mnt/sdcard/ECG/SVM2/groundtruth105.dat");
			reader = new BufferedReader(fr);
			int i = 1;
			while (i < beg) {
				reader.readLine();
				i++;
			}
			i = 0;
			String tmp;
			// String[][] ps = new String[end-beg+1][2];
			while (i < end - beg + 1 && (tmp = reader.readLine()) != null) {
				// ps[i][0]=tmp.split("\\t")[0];
				// ps[i][1]=tmp.split("\\t")[1];
				// pointArray[i] = new PointF();
				// pointArray[i].x = Float.parseFloat(ps[i][0]);
				// pointArray[i].y = Float.parseFloat(ps[i][1]);
				// pointArray[i].y = pointArray[i].y / 2.4f / 1000;
				groundtruthArray[i] = Integer.parseInt(tmp);
				i++;
			}

		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

	}
}

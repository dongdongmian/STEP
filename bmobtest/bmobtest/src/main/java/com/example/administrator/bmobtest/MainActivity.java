package com.example.administrator.bmobtest;

import android.app.AlertDialog;
import android.os.Bundle;
import android.support.v7.app.ActionBarActivity;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.EditText;
import android.widget.Toast;

import java.util.List;

import cn.bmob.push.BmobPush;
import cn.bmob.v3.Bmob;
import cn.bmob.v3.BmobInstallation;
import cn.bmob.v3.BmobQuery;
import cn.bmob.v3.listener.FindListener;
import cn.bmob.v3.listener.SaveListener;


public class MainActivity extends ActionBarActivity {

    private EditText mName,mFeedback,mQuery;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Bmob.initialize(this,"ecdba10603e9ecb51da7439e8c687baa");
        BmobInstallation.getCurrentInstallation(this);
        BmobPush.startWork(this,"ecdba10603e9ecb51da7439e8c687baa");

        mName =(EditText) findViewById(R.id.name);
        mFeedback = (EditText) findViewById(R.id.feedback);
        mQuery = (EditText) findViewById(R.id.q_name);
    }

    public void query(View view){
        String q_name = mQuery.getText().toString();
        if(q_name.equals("")){
            return;
        }
        BmobQuery<BmobTest> query = new BmobQuery<BmobTest>();
        query.addWhereEqualTo("name",q_name);
        query.findObjects(MainActivity.this,new FindListener<BmobTest>() {
            @Override
            public void onSuccess(List<BmobTest> bmobTests) {
                AlertDialog.Builder builder = new AlertDialog.Builder(MainActivity.this);
                builder.setTitle("Query");
                String str = "";
                for(BmobTest BmobTest : bmobTests){
                    str +=  BmobTest.getName()+":"+BmobTest.getFeedback()+"\n";
                }
                builder.setMessage(str);
                builder.create().show();
            }

            @Override
            public void onError(int i, String s) {
                Toast.makeText(MainActivity.this,"query failure",Toast.LENGTH_LONG).show();
            }
        });
    }

    public void submit(View view) {
        String name = mName.getText().toString();
        String feedback = mFeedback.getText().toString();

        if(name.equals("") || feedback.equals("")){
            return;
        }
        BmobTest bmob = new BmobTest();
        bmob.setName(name);
        bmob.setFeedback(feedback);
        bmob.save(MainActivity.this, new SaveListener() {
            @Override
            public void onSuccess() {
                Toast.makeText(MainActivity.this,"submit success",Toast.LENGTH_LONG).show();
            }

            @Override
            public void onFailure(int i, String s) {
                Toast.makeText(MainActivity.this,"submit failure",Toast.LENGTH_LONG).show();
            }
        });
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }
}

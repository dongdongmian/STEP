package com.example.administrator.bmobtest;

import cn.bmob.v3.BmobObject;

/**
 * Created by Administrator on 2015/2/24.
 */
public class BmobTest extends BmobObject {
    private String name;
    private String feedback;

    public String getFeedback() {
        return feedback;
    }

    public void setFeedback(String feedback) {
        this.feedback = feedback;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }
}

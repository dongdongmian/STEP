
import javax.xml.parsers.*;
import javax.xml.transform.*;
import org.w3c.dom.*;
import java.io.*;
import java.util.*;
import org.xml.sax.*;
import org.xml.sax.helpers.DefaultHandler;

public class Filmxml{
	private String xmlFileName;
	
	
	//创建一个类，将要解析的文档作为参数
	public Filmxml(String fileName){
		this.xmlFileName = fileName;
	}	
		
	//定义用ShowListBySAX（）的方法解析文档
	public void ShowListBySAX(){
		try{
			//创建SAX解析工厂对象
			SAXParserFactory spf = SAXParserFactory.newInstance();
			//创建SAX解析器对象
			SAXParser sp = spf.newSAXParser();
			//创建SAX处理器对象
			SAXHandler handle = new SAXHandler();
			//调用解析器对象parse方法解析XMl文档
			sp.parse(xmlFileName,handle);
		}catch(Exception e){
			e.printStackTrace();
		}
	}
	
	public static void main(String[] args){
		Filmxml fl = new Filmxml("film.xml");
		//调用方法开始进行解析
		fl.ShowListBySAX();
	}
	
	//解析器事件处理类，用于获取XML文档数据
	public class SAXHandler extends DefaultHandler{
		private Vector v = new Vector();
		private String currentElement = null;
		private String currentValue = null;
		
		public void startDocument() throws SAXException{
			System.out.println("SAX 事件：startDocument");
		}
		
		public void endDocument() throws SAXException {
			System.out.println("SAX 事件：endDocument");
		}
		
		public void setTablefVector(Vector vector){
			this.v = vector;
		}
		
		public Vector getVector(){
			return v;
		}
		
		public void startElement(String namespaceURL,String localName,String qName,Attributes attrs)
			throws SAXException{
			currentElement = qName;
			System.out.println("SAX 事件:starElement -> "+qName);
			for(int i = 0;i<attrs.getLength();i++){
				System.out.println("属性值： "+attrs.getValue(i));
			}
		}
		
		public void characters(char[] ch,int start,int length) throws SAXException{
			currentValue = new String(ch,start,length);
			System.out.println("字符： "+currentValue);
		}
		
		public void endElement(String namespaceURl,String localName,String qName) throws SAXException{
			if(currentElement.equals("Name")){
				v.add(currentValue);
			}
			System.out.println("SAX 事件 ：endElement ->" +qName);
		}
	}
}
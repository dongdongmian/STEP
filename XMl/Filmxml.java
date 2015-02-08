
import javax.xml.parsers.*;
import javax.xml.transform.*;
import org.w3c.dom.*;
import java.io.*;
import java.util.*;
import org.xml.sax.*;
import org.xml.sax.helpers.DefaultHandler;

public class Filmxml{
	private String xmlFileName;
	
	
	//����һ���࣬��Ҫ�������ĵ���Ϊ����
	public Filmxml(String fileName){
		this.xmlFileName = fileName;
	}	
		
	//������ShowListBySAX�����ķ��������ĵ�
	public void ShowListBySAX(){
		try{
			//����SAX������������
			SAXParserFactory spf = SAXParserFactory.newInstance();
			//����SAX����������
			SAXParser sp = spf.newSAXParser();
			//����SAX����������
			SAXHandler handle = new SAXHandler();
			//���ý���������parse��������XMl�ĵ�
			sp.parse(xmlFileName,handle);
		}catch(Exception e){
			e.printStackTrace();
		}
	}
	
	public static void main(String[] args){
		Filmxml fl = new Filmxml("film.xml");
		//���÷�����ʼ���н���
		fl.ShowListBySAX();
	}
	
	//�������¼������࣬���ڻ�ȡXML�ĵ�����
	public class SAXHandler extends DefaultHandler{
		private Vector v = new Vector();
		private String currentElement = null;
		private String currentValue = null;
		
		public void startDocument() throws SAXException{
			System.out.println("SAX �¼���startDocument");
		}
		
		public void endDocument() throws SAXException {
			System.out.println("SAX �¼���endDocument");
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
			System.out.println("SAX �¼�:starElement -> "+qName);
			for(int i = 0;i<attrs.getLength();i++){
				System.out.println("����ֵ�� "+attrs.getValue(i));
			}
		}
		
		public void characters(char[] ch,int start,int length) throws SAXException{
			currentValue = new String(ch,start,length);
			System.out.println("�ַ��� "+currentValue);
		}
		
		public void endElement(String namespaceURl,String localName,String qName) throws SAXException{
			if(currentElement.equals("Name")){
				v.add(currentValue);
			}
			System.out.println("SAX �¼� ��endElement ->" +qName);
		}
	}
}
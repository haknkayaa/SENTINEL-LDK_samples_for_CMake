
public class Customer {
	private String firstName = null;
	private String lastName = null;
	private String email = null;
	
	/**
	 * constructor with mandatory parameters args
	 * @param firstName
	 * @param lastName
	 * @param email
	 */
	public Customer(String firstName,String lastName,String email){
		this.email = email;
		this.firstName = firstName;
		this.lastName = lastName;
	}
	
	
	public String toWSXml(){
		StringBuffer sb = new StringBuffer();
		
		sb.append("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
		sb.append("<customer xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n");
		sb.append("    xsi:noNamespaceSchemaLocation=\"../xsd/customer.xsd\">\n");
		sb.append("    <type>ind</type>\n");
		sb.append("    <name></name>\n");
		sb.append("    <enabled>true</enabled>\n");
		sb.append("    <description></description>\n");
		sb.append("    <crmId></crmId>\n");
		sb.append("    <refId></refId>\n");
		sb.append("    <phone></phone>\n");
		sb.append("    <fax></fax>\n");
		sb.append("    <addresses>\n");
		sb.append("        <shippingSameAsBilling>true</shippingSameAsBilling>\n");
		sb.append("        <address>\n");
		sb.append("                  <type>billing</type>\n");
		sb.append("                  <street></street>\n");
		sb.append("                  <city></city>\n");
		sb.append("                  <state></state>\n");
		sb.append("                  <country></country>\n");
		sb.append("                  <zip></zip>\n");
		sb.append("        </address>\n");
		sb.append("    </addresses>\n");
		sb.append("    <defaultContact>\n");
		sb.append("        <emailId>"+email+"</emailId>\n");
		sb.append("        <firstName><![CDATA["+firstName+"]]></firstName>\n");
		sb.append("        <middleName></middleName>\n");
		sb.append("       <lastName><![CDATA["+lastName+"]]></lastName>\n");
		sb.append("        <locale></locale>\n");
		sb.append("    </defaultContact>\n");
		sb.append("</customer>\n");


		
		return sb.toString();
	}
	
}

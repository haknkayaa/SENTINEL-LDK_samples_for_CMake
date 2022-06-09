import java.io.BufferedReader;
import java.io.ByteArrayInputStream;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.security.SecureRandom;
import java.security.cert.CertificateException;
import java.security.cert.X509Certificate;
import java.util.List;
import java.util.Map;

import javax.net.ssl.HostnameVerifier;
import javax.net.ssl.HttpsURLConnection;
import javax.net.ssl.KeyManager;
import javax.net.ssl.SSLContext;
import javax.net.ssl.SSLSession;
import javax.net.ssl.TrustManager;
import javax.net.ssl.X509TrustManager;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import Aladdin.Hasp;
import Aladdin.HaspStatus;

/**
 * This class contains all the business logic needed for the activation.<br/>
 * <p/>
 * This class also handles self signed certificate on https connections.
 * <p/>
 * This class is dependent on hasp-srm-api.jar.
 */
public class Activation {
	/** Record created indication on WS PUT calls */
	public final static int CREATED = 201;
	/** Required header for WS calls */
	private final static String VERSION = "application/vnd.ems.v12";
	/** Runtime query scope */
	protected final static String KEY_SCOPE = "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n"
			+ "<haspscope>\n"
			+ "<license_manager hostname=\"localhost\" />\n"
			+ "</haspscope>\n";
	/** SL-Legacy C2V not used for V2CP activation**/
	protected final static String SCOPE_WITHOUT_SL_LEGACY = 
		      "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>"+
		      "<haspscope>"+
		      " <license_manager hostname=\"localhost\" >"+
		      " <hasp key_type=\"~SL-Legacy\" />"+
		      " </license_manager>"+
		      "</haspscope>";
	/** Runtime query view */
	protected final static String KEY_VIEW = "<haspformat format=\"host_fingerprint\"/>";

	protected final static String KEY_VIEW_FOR_V2CP = "<haspformat root=\"hasp_info\">  "
			+ "<hasp>      "
			+ "  <attribute name=\"id\" /> "
			+ "  <attribute name=\"type\" />    " + "</hasp>" + "</haspformat>";

	protected final static String KEYID_SCOPE1 = "	<haspscope>     <hasp id=\"";

	protected final static String KEYID_SCOPE2 = "\" /></haspscope>";

	protected final static String KEY_C2V_FORMAT = "<haspformat format=\"updateinfo\"/>";

	/** WS uri */
	public static final String PK_LOGIN_URL = "ems/v21/ws/loginByProductKey.ws";
	public static final String REGISTER_URL = "ems/v21/ws/customer.ws";
	public static final String V2CP_URL = "/ems/v78/ws/activation/target.ws";

	/** Error message constants */
	public static final String MSG_BAD_CONNECTION = "Sentinel EMS is not available for the specified URL";
	public static final String FAILS_WITH_CODE = "Fails with status code ";
	public static final String TOO_MANY_KEYS = "Too many keys connected.";
	public static final String NO_KEYS = "No key connected.";
	public static final String ERROR_CODE = "Error - code ";
	public static final String SERVER_RETURNED_ERROR = "server returned error code";

	public static final Log log = LogFactory.getLog(Activation.class);

	/**
	 * Handle self signed certificates
	 */
	static {
		try {
			SSLContext ctx = SSLContext.getInstance("TLS");
			ctx.init(new KeyManager[0],
					new TrustManager[] { new DefaultTrustManager() },
					new SecureRandom());
			SSLContext.setDefault(ctx);
		} catch (Exception e) {
			e.printStackTrace();
			log.error("Error: " + e.getMessage(), e);
		}
	}

	/**
	 * Creates a connection either http or ssl depending on the protocol
	 * specified in the URL
	 * 
	 * @param url
	 * @param isHttps
	 * @return
	 * @throws Exception
	 */
	protected static HttpURLConnection createConnection(URL url, boolean isHttps)
			throws Exception {
		if (!isHttps) {
			return (HttpURLConnection) url.openConnection();
		} else {
			HttpsURLConnection conn = (HttpsURLConnection) url.openConnection();

			// for handling user generated certificates, which are not CA
			// verified
			// so we override the HostnameVerifier in the connection so host
			// will not
			// be verified with the certificate host
			conn.setHostnameVerifier(new HostnameVerifier() {
				@Override
				public boolean verify(String arg0, SSLSession arg1) {
					return true;
				}
			});
			return (HttpURLConnection) conn;
		}
	}

	/**
	 * Updates the key with the license (v2c)
	 * 
	 * @param v2c
	 * @return
	 * @throws Exception
	 */
	public static void updateKeyWithLicense(String v2c, String vendorCode)
			throws Exception {
		Hasp hasp = new Hasp(Hasp.HASP_DEFAULT_FID);
		hasp.update(v2c);
		int status = hasp.getLastError();
		if (status != HaspStatus.HASP_STATUS_OK) {
			log.error("Error: updateKeyWithLicense Fails with status code "
					+ status);
			throw new Exception("Fails with status code " + status);
		}
	}

	/**
	 * invokes the EMS login web service
	 * 
	 * @param serverUrl
	 * @param product key
	 * 
	 * @return
	 */
	public static String[] customerLogin(String serverUrl, String pk)
			throws Exception {
		URL url = new URL(serverUrl + '/' + PK_LOGIN_URL);
		OutputStream out = null;
		BufferedReader in = null;
		boolean https = isHttps(serverUrl);
		HttpURLConnection conn = null;
		// byte[] iso88591Data = pk.getBytes("ISO-8859-1");
		// String tmp = new String(iso88591Data);
		String body = "productKey=" + pk;
		BufferedReader errIn = null;

		try {
			conn = createConnection(url, https);
			conn.setDoInput(true);
			conn.setDoOutput(true);
			conn.setRequestMethod("POST");
			conn.setRequestProperty("Accept", VERSION);
			// conn.setRequestProperty("Content-Type",
			// "text/plain; charset=utf-8");
			out = conn.getOutputStream();
			out.write(body.getBytes());
			out.flush();
			in = new BufferedReader(
					new InputStreamReader(conn.getInputStream()));
			StringBuffer sb = new StringBuffer();
			String line = null;
			while ((line = in.readLine()) != null) {
				sb.append(line);
				sb.append('\n');
			}
			return extractJsessionIdFromResponese(sb.toString());
		} catch (Exception e) {
			log.error("Error customerLogin : " + e.getMessage(), e);
			if (conn != null) {
				try {
					int respCode = conn.getResponseCode();
					log.info("customerLogin returned http code = " + respCode);
					if (respCode == 405) {
						throw new Exception(MSG_BAD_CONNECTION);
					}
					errIn = new BufferedReader(new InputStreamReader(
							conn.getErrorStream()));
				} catch (Exception ex) {
					throw new Exception(MSG_BAD_CONNECTION);
				}

				StringBuffer sb = new StringBuffer();
				String line = null;
				while ((line = errIn.readLine()) != null) {
					sb.append(line);
					sb.append('\n');
				}
				String errMsg = sb.toString();
				throw new Exception(errMsg, e);
			}
			throw e;
		} finally {
			if (out != null)
				out.close();
			if (in != null)
				in.close();
			if (conn != null)
				conn.disconnect();
		}

	}

	/**
	 * extracts the jsessionid and the registration flag from the login response
	 * 
	 * @param xml
	 * @return
	 */
	protected static String[] extractJsessionIdFromResponese(String xml)
			throws Exception {
		String[] loginRes = new String[3];
		if (xml.toLowerCase().contains("<html>")) {
			return null;
		}
		InputStream is = new ByteArrayInputStream(xml.getBytes("UTF-8"));
		DocumentBuilder builder = DocumentBuilderFactory.newInstance()
				.newDocumentBuilder();
		Document doc = builder.parse(is);
		// get the jsession id
		NodeList nl = doc.getElementsByTagName("sessionId");
		String sessionId = null;
		if (nl != null && nl.getLength() > 0) {
			for (int i = 0; i < nl.getLength(); ++i) {
				if (nl.item(0).getNodeType() == Node.ELEMENT_NODE) {
					sessionId = ((Element) nl.item(0)).getTextContent();
					loginRes[0] = sessionId;
				}
			}
		}
		// see if registration is required
		NodeList nlReg = doc.getElementsByTagName("regRequired");
		String reg = null;
		if (nlReg != null && nlReg.getLength() > 0) {
			for (int i = 0; i < nlReg.getLength(); ++i) {
				if (nlReg.item(0).getNodeType() == Node.ELEMENT_NODE) {
					reg = ((Element) nlReg.item(0)).getTextContent();
					loginRes[1] = reg;
				}
			}
		}

		// see if redirect To UserReg needed
		NodeList nlRedirectToUserReg = doc
				.getElementsByTagName("redirectToUserReg");
		String redirectToUserReg = null;
		if (nlRedirectToUserReg != null && nlRedirectToUserReg.getLength() > 0) {
			for (int i = 0; i < nlRedirectToUserReg.getLength(); ++i) {
				if (nlRedirectToUserReg.item(0).getNodeType() == Node.ELEMENT_NODE) {
					redirectToUserReg = ((Element) nlRedirectToUserReg.item(0))
							.getTextContent();
					loginRes[2] = redirectToUserReg;
				}
			}
		}

		return loginRes;
	}

	/**
	 * 
	 * @param serverUrl
	 * @param jsessionId
	 * @param customer
	 * @return
	 * @throws Exception
	 */
	public static int register(String serverUrl, String jsessionId,
			Customer customer) throws Exception {
		URL url = new URL(serverUrl + REGISTER_URL);
		OutputStream out = null;
		BufferedReader in = null;
		boolean https = isHttps(serverUrl);
		HttpURLConnection conn = null;
		String body = customer.toWSXml();
		BufferedReader errIn = null;
		try {
			conn = createConnection(url, https);
			conn.setDoInput(true);
			conn.setDoOutput(true);
			conn.setRequestMethod("PUT");
			conn.setRequestProperty("Accept", VERSION);
			conn.setRequestProperty("Cookie", "JSESSIONID=" + jsessionId);
			conn.setRequestProperty("Content-Type", "text/xml; charset=utf-8");
			out = conn.getOutputStream();
			out.write(body.getBytes("utf-8"));
			out.flush();
			in = new BufferedReader(
					new InputStreamReader(conn.getInputStream()));
			in.readLine();
			return conn.getResponseCode();
		} catch (Exception e) {
			log.error("Error registering : " + e.getMessage(), e);
			if (conn != null) {
				errIn = new BufferedReader(new InputStreamReader(
						conn.getErrorStream()));
				StringBuffer sb = new StringBuffer();
				String line = null;
				while ((line = errIn.readLine()) != null) {
					sb.append(line);
					sb.append('\n');
				}
				String errMsg = sb.toString();
				throw new Exception(errMsg, e);
			}
			throw e;
		} finally {
			if (out != null)
				out.close();
			if (in != null)
				in.close();
			if (errIn != null)
				errIn.close();
			if (conn != null)
				conn.disconnect();
		}

	}

	/**
	 * 
	 * @param vendorCode
	 * @return
	 * @throws Exception
	 */
	public static String readListOfKeys(String vendorCode) throws Exception {
		Hasp hasp = new Hasp(Hasp.HASP_DEFAULT_FID);
		String infos = hasp.getInfo(SCOPE_WITHOUT_SL_LEGACY, KEY_VIEW_FOR_V2CP, vendorCode);
		int status = hasp.getLastError();
		log.info("reading C2v from key status code = " + status);
		switch (status) {
		case HaspStatus.HASP_STATUS_OK:
			return infos;
		case HaspStatus.HASP_NO_API_DYLIB:
			throw new Exception("API for this vendor not found.");
		case HaspStatus.HASP_TOO_MANY_KEYS:
			throw new Exception("Too many keys connected.");
		case HaspStatus.HASP_SCOPE_RESULTS_EMPTY:
			throw new Exception("No key connected.");
		default:
			throw new Exception("Error - code " + status);
		}
	}

	/**
	 * 
	 * @param vendorCode
	 * @return
	 * @throws Exception
	 */
	public static String fetchC2VforKey(String haspId, String vendorCode)
			throws Exception {
		Hasp hasp = new Hasp(Hasp.HASP_DEFAULT_FID);
		String infos = hasp.getInfo(KEYID_SCOPE1 + haspId + KEYID_SCOPE2,
				KEY_C2V_FORMAT, vendorCode);
		int status = hasp.getLastError();
		log.info("reading C2v from key status code = " + status);
		switch (status) {
		case HaspStatus.HASP_STATUS_OK:
			return infos;
		case HaspStatus.HASP_NO_API_DYLIB:
			throw new Exception("API for this vendor not found.");
		case HaspStatus.HASP_TOO_MANY_KEYS:
			throw new Exception("Too many keys connected.");
		case HaspStatus.HASP_SCOPE_RESULTS_EMPTY:
			throw new Exception("No key connected.");
		default:
			throw new Exception("Error - code " + status);
		}
	}

	/**
	 * checks http/https protocol
	 * 
	 * @param serverUrl
	 * @return
	 */
	protected static boolean isHttps(String serverUrl) {
		if (serverUrl.toLowerCase().startsWith("https")) {
			return true;
		} else {
			return false;
		}
	}

	/**
	 * 
	 * @param jsessionId
	 * @param c2v
	 * @param product
	 *            key
	 * @return
	 */
	public static String getLicense(String serverUrl, String jsessionId,
			String c2v, String pk) throws Exception {
		String requestBody = generateLicenseRequestXml(c2v);
		URL url = new URL(serverUrl + "/ems/v21/ws/productKey/" + pk
				+ "/activation.ws");
		OutputStream out = null;
		BufferedReader in = null;
		boolean https = isHttps(serverUrl);
		HttpURLConnection conn = null;
		BufferedReader errIn = null;

		try {
			conn = createConnection(url, https);
			conn.setDoInput(true);
			conn.setDoOutput(true);
			conn.setRequestMethod("POST");
			conn.setRequestProperty("Accept", VERSION);
			conn.setRequestProperty("Cookie", "JSESSIONID=" + jsessionId);
			conn.setRequestProperty("Content-type", "application/xml");
			out = conn.getOutputStream();
			out.write(requestBody.getBytes());
			out.flush();
			in = new BufferedReader(
					new InputStreamReader(conn.getInputStream()));
			if (conn.getResponseCode() == 200) {
				StringBuffer sb = new StringBuffer();
				String line = null;
				while ((line = in.readLine()) != null) {
					sb.append(line);
					sb.append('\n');
				}
				String responseXml = sb.toString();
				return exctractV2CFromResponseXml(responseXml);
			} else {
				throw new Exception("server returned error code "
						+ conn.getResponseCode());
			}
		} catch (Exception e) {
			log.error("Error getting license " + e.getMessage(), e);
			if (conn != null) {
				errIn = new BufferedReader(new InputStreamReader(
						conn.getErrorStream()));
				StringBuffer sb = new StringBuffer();
				String line = null;
				while ((line = errIn.readLine()) != null) {
					sb.append(line);
					sb.append('\n');
				}
				String errMsg = sb.toString();
				throw new Exception(errMsg, e);
			}
			throw e;
		} finally {
			if (out != null)
				out.close();
			if (in != null)
				in.close();
			if (errIn != null)
				errIn.close();
			if (conn != null)
				conn.disconnect();
		}
	}

	/**
	 * 
	 * @param vendorCode
	 * @return
	 * @throws Exception
	 */
	public static String readC2vFromKey(String vendorCode) throws Exception {
		Hasp hasp = new Hasp(Hasp.HASP_DEFAULT_FID);
		String infos = hasp.getInfo(KEY_SCOPE, KEY_VIEW, vendorCode);
		int status = hasp.getLastError();
		log.info("reading C2v from key status code = " + status);
		switch (status) {
		case HaspStatus.HASP_STATUS_OK:
			return infos;
		case HaspStatus.HASP_NO_API_DYLIB:
			throw new Exception("API for this vendor not found.");
		case HaspStatus.HASP_TOO_MANY_KEYS:
			throw new Exception("Too many keys connected.");
		case HaspStatus.HASP_SCOPE_RESULTS_EMPTY:
			throw new Exception("No key connected.");
		default:
			throw new Exception("Error - code " + status);
		}
	}

	/**
	 *  Fetch value of Polling Frequency, set by ISV in response header.
	 */
	public static int fetchPollingFreqFromResponse(HttpURLConnection conn)
	{
		Map<String, List<String>> headerFieldsMap = conn.getHeaderFields();
		List<String> pollFreq = (List<String>)headerFieldsMap.get("polling-frequency");
		int pollingFreqValue = 0;
		if(pollFreq != null)
		{
			pollingFreqValue = Integer.valueOf(pollFreq.get(0));
		}
		
		return pollingFreqValue;
	}

	/**
	 * 
	 * @param jsessionId
	 * @param c2v
	 * @param product
	 *            key
	 * @return
	 */
	public static String geV2CPtLicense(String serverUrl, String c2v)
			throws Exception {
		URL url = new URL(serverUrl + V2CP_URL);
		OutputStream out = null;
		BufferedReader in = null;
		boolean https = isHttps(serverUrl);
		HttpURLConnection conn = null;
		BufferedReader errIn = null;
		String responseXml = null;
		try {
			conn = createConnection(url, https);
			conn.setDoInput(true);
			conn.setDoOutput(true);
			conn.setRequestMethod("POST");
			conn.setRequestProperty("Accept", VERSION);
			conn.setRequestProperty("Content-type", "application/xml");
			out = conn.getOutputStream();
			out.write(c2v.getBytes());
			out.flush();
			in = new BufferedReader(
					new InputStreamReader(conn.getInputStream()));
			if (conn.getResponseCode() == 200) {
				StringBuffer sb = new StringBuffer();
				String line = null;
				while ((line = in.readLine()) != null) {
					sb.append(line);
					sb.append('\n');
				}
				responseXml = sb.toString();
				System.out.println("Web Service Response : " + responseXml);
				
				/*
				 * In case of successful response, polling frequency value can be used to know the suggested time
				 * after which this web-service shall be called again to check for availability of next update.
				 * This value can be set by ISV in Sentinel EMS Admin console. 
				 * Polling Frequency value 0 indicates that this value need to be ignored.
				 * 
				 * For more information refer Sentinel EMS documentation.
				 */
				int pollingFreq = fetchPollingFreqFromResponse(conn);
				
				return responseXml;
			} else {
				throw new Exception("server returned error code "
						+ conn.getResponseCode());
			}
		} catch (Exception e) {
			log.error("Error getting license " + e.getMessage(), e);
			if (conn != null) {
				errIn = new BufferedReader(new InputStreamReader(
						conn.getErrorStream()));
				StringBuffer sb = new StringBuffer();
				String line = null;
				while ((line = errIn.readLine()) != null) {
					sb.append(line);
					sb.append('\n');
				}
				String errMsg = sb.toString();
				throw new Exception(errMsg, e);
			}
			throw e;
		} finally {
			if (out != null)
				out.close();
			if (in != null)
				in.close();
			if (errIn != null)
				errIn.close();
			if (conn != null)
				conn.disconnect();
		}
	}

	/**
	 * 
	 * @param responseXml
	 * @return
	 * @throws Exception
	 */
	protected static String exctractV2CFromResponseXml(String responseXml)
			throws Exception {
		InputStream is = new ByteArrayInputStream(responseXml.getBytes("UTF-8"));
		DocumentBuilder builder = DocumentBuilderFactory.newInstance()
				.newDocumentBuilder();
		Document doc = builder.parse(is);
		// get the jsession id
		NodeList nl = doc.getElementsByTagName("activationString");
		String activationString = null;
		if (nl != null && nl.getLength() > 0) {
			for (int i = 0; i < nl.getLength(); ++i) {
				if (nl.item(0).getNodeType() == Node.ELEMENT_NODE) {
					activationString = ((Element) nl.item(0)).getTextContent();
					return activationString;
				}
			}
		}

		return null;
	}

	/**
	 * Creates an WS request xml containing the c2v
	 * 
	 * @param c2v
	 * @return
	 */
	protected static String generateLicenseRequestXml(String c2v) {
		StringBuffer sb = new StringBuffer();
		sb.append("<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n");
		sb.append("<activation xsi:noNamespaceSchemaLocation=\"License.xsd\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">\n");
		sb.append("<activationInput>\n");
		sb.append("<activationAttribute>\n");
		sb.append("<attributeValue>\n");
		sb.append("<![CDATA[");
		sb.append(c2v);
		sb.append("]]> \n");
		sb.append("</attributeValue>\n");
		sb.append("<attributeName>C2V</attributeName> \n");
		sb.append(" </activationAttribute>\n");
		sb.append("<comments></comments> \n");
		sb.append("</activationInput>\n");
		sb.append("</activation>\n");

		return sb.toString();
	}

	/**
	 * For handling user generated certificates
	 * 
	 *
	 */
	private static class DefaultTrustManager implements X509TrustManager {
		@Override
		public void checkClientTrusted(X509Certificate[] arg0, String arg1)
				throws CertificateException {
		}

		@Override
		public void checkServerTrusted(X509Certificate[] arg0, String arg1)
				throws CertificateException {
		}

		@Override
		public X509Certificate[] getAcceptedIssuers() {
			return null;
		}
	}

}

import java.io.FileInputStream;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.Properties;
import java.util.Scanner;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

/**
 * Activation Sample class.<br/>
 * This class handles the only the UI part, check out the Activation class<br/>
 * which contains all the business logic for activation.
 * <p/>
 * 
 * The sample reads its init parameters from the activation.properties class.<br/>
 * you can modify the entries in the properties file if needed.
 * 
 * 
 *
 */
public class ActivationSample {
	/** input data properties keys */
	public static final String PROP_KEY_URL = "url";
	public static final String PROP_KEY_PRODUCT_KEY = "product_key";

	public static final String PROP_KEY_FIRST_NAME = "first_name";
	public static final String PROP_KEY_LAST_NAME = "last_name";
	public static final String PROP_KEY_EMAIL = "email";

	/** registration indication flags values */
	public static final int REG_NOTREQUIRED = 1;
	public static final int REG_DESIRED = 2;
	public static final int REG_MANDATORY = 3;

	public static final String VENDOR_CODE = "AzIceaqfA1hX5wS+M8cGnYh5ceevUnOZIzJBbXFD6dgf3tBkb9cvUF/Tkd/iKu2fsg9wAysYKw7RMAsVvIp4KcXle/v1RaXrLVnNBJ2H2DmrbUMOZbQUFXe698qmJsqNpLXRA367xpZ54i8kC5DTXwDhfxWTOZrBrh5sRKHcoVLumztIQjgWh37AzmSd1bLOfUGI0xjAL9zJWO3fRaeB0NS2KlmoKaVT5Y04zZEc06waU2r6AU2Dc4uipJqJmObqKM+tfNKAS0rZr5IudRiC7pUwnmtaHRe5fgSI8M7yvypvm+13Wm4Gwd4VnYiZvSxf8ImN3ZOG9wEzfyMIlH2+rKPUVHI+igsqla0Wd9m7ZUR9vFotj1uYV0OzG7hX0+huN2E/IdgLDjbiapj1e2fKHrMmGFaIvI6xzzJIQJF9GiRZ7+0jNFLKSyzX/K3JAyFrIPObfwM+y+zAgE1sWcZ1YnuBhICyRHBhaJDKIZL8MywrEfB2yF+R3k9wFG1oN48gSLyfrfEKuB/qgNp+BeTruWUk0AwRE9XVMUuRbjpxa4YA67SKunFEgFGgUfHBeHJTivvUl0u4Dki1UKAT973P+nXy2O0u239If/kRpNUVhMg8kpk7s8i6Arp7l/705/bLCx4kN5hHHSXIqkiG9tHdeNV8VYo5+72hgaCx3/uVoVLmtvxbOIvo120uTJbuLVTvT8KtsOlb3DxwUrwLzaEMoAQAFk6Q9bNipHxfkRQER4kR7IYTMzSoW5mxh3H9O8Ge5BqVeYMEW36q9wnOYfxOLNw6yQMf8f9sJN4KhZty02xm707S7VEfJJ1KNq7b5pP/3RjE0IKtB2gE6vAPRvRLzEohu0m7q1aUp8wAvSiqjZy7FLaTtLEApXYvLvz6PEJdj4TegCZugj7c8bIOEqLXmloZ6EgVnjQ7/ttys7VFITB3mazzFiyQuKf4J6+b/a/Y";

	protected final Log log = LogFactory.getLog(this.getClass());

	/** input data */
	protected Properties props = null;

	/** the jsessionid for initiating WS calls to the EMS server */
	protected String jSessionId = null;

	/** flag indicating if registration is needed */
	protected int registrationStat = 0;
	protected String redirectToUserReg = "";

	/**
	 * Displays Welcome Message
	 * @return
	 */
	protected String createWelcomeMessage() {
		StringBuffer sb = new StringBuffer();
		sb.append("\nWelcome to the Sentinel LDK Activation Sample\n");
		sb.append("\n");
		sb.append("If no key exists on this machine, use the \nProduct Key Activation Web Service.\n");
		sb.append("\nIf an HL or SL key exists on this machine, use the \nGet Pending Updates by KeyID Web Service.\n");		
		sb.append("\nEnter '1' for Product Key Activation\n");
		sb.append("\nOR\n");
		sb.append("\nEnter '2' for Get Pending Updates by KeyID\n");
		return sb.toString();
	}

	/**
	 * Displays message for V2CP flow.
	 * @return
	 */
	protected String createMessageForV2CP() {
		StringBuffer sb = new StringBuffer();
		sb.append("\nThis sample demonstrates the steps required to get pending updates for\n HL or SL(AdminMode/UserMode) Key and apply them. \n");
		sb.append("\nThe required steps are:\n");
		sb.append("1. Use the hasp_get_info() API to retrieve the C2V for the selected Key ID (hasp id). \n");
		sb.append("2. Use the get pending updates Web Service to get a V2CP file.\n");
		sb.append("3. Apply the V2CP file using the hasp_update() API.\n");
		sb.append("\n");
		sb.append("Press <Enter> to continue.");
		return sb.toString();
	}

	/**
	 * Displays message for activation flow.
	 * @return
	 */
	protected String createMessageForPK() {
		StringBuffer sb = new StringBuffer();
		sb.append("\nThis sample demonstrates the steps required to activate an SL license and\n register an end user.\n");
		sb.append("\nThe required steps are:\n");
		sb.append("1. Use the \"hasp_get_info\" function to retrieve a fingerprint \nof the end user's computer. \n");
		sb.append("2. The Sample script uses the fingerprint together with a Product Key in Sentinel EMS\n to generate an Activation V2C file.\n");
		sb.append("3.  Apply the V2C file to the end user's computer using the \n\"hasp_update\" function.\n");
		sb.append("\n");
		sb.append("Note:\n");
		sb.append("Sentinel EMS URL, Entitlement Product Key and Registration info are \nconfigurable in the activation.properties file.\n");
		sb.append("\n");
		sb.append("Press <Enter> to continue.");
		return sb.toString();
	}

	/**
	 * 
	 * @return
	 */
	protected boolean checkForEnter() {
		InputStream in = null;
		try {
			byte[] b = new byte[10];
			in = System.in;
			int i = in.read(b);
			char c = (char) b[0];

			if (c == '\n' || c == '\r') {
				return true;
			}
		} catch (Exception e) {
			log.error("Exception thrown in checkForEnter " + e.getMessage(), e);
		}
		return false;
	}

	/**
	 * Read user input.
	 * @return
	 */
	protected String checkForActivationType() {
		Scanner in = new Scanner(System.in);
		String s = in.nextLine();
		return s;
	}

	/**
	 * Perform Product key activation or V2CP 
	 * update based on user input.
	 */
	public void activate() {
		System.out.println(createWelcomeMessage());
		String activationType = checkForActivationType();

		props = new Properties();
		try {
			loadProps();

			if (activationType.equalsIgnoreCase("1")) {
				System.out.println(createMessageForPK());
				boolean continueActivation = checkForEnter();
				if (!continueActivation) {
					return;
				}
				if (customerLogin()) {
					log.info("Registering customer...");
					// check if registration is required
					if (registrationStat == REG_MANDATORY
							|| registrationStat == REG_DESIRED) {
						if (redirectToUserReg.equalsIgnoreCase("true")) {
							register();
						} else {
							System.out
									.println("Registration has already been performed for this Product Key.");
						}
					}
					log.info("activating...");
					doActivation();
				}
			} else if (activationType.equalsIgnoreCase("2")) {
				System.out.println(createMessageForV2CP());
				boolean continueActivation = checkForEnter();
				if (!continueActivation) {
					return;
				}
				doV2CPActivation();
			} else {
				System.out.println("WRONG INPUT PROVIDED.");
				return;
			}

		} catch (Exception e) {
			log.error("Error activating: " + e.getMessage(), e);
			System.out.println(e.getMessage());
		}
	}

	/**
	 * Registers the customer
	 * 
	 * @return true if registration succeeds
	 */
	protected boolean register() {
		System.out.println("Registering...");
		String firstName = props.getProperty(PROP_KEY_FIRST_NAME);
		String lastName = props.getProperty(PROP_KEY_LAST_NAME);
		String email = props.getProperty(PROP_KEY_EMAIL);
		String url = props.getProperty(PROP_KEY_URL);
		// validate input params
		if (firstName == null || lastName == null || email == null
				|| firstName.equals("") || lastName.equals("")
				|| email.equals("")) {
			System.out
					.println("Some of the customer registration information was not set.");
			System.out.println("Check the activation.properties file.");
			log.info("Some of the customer registration information was not set. Check the activation.properties file.");
			return false;
		}
		// create a customer object with mandatory fields
		Customer customer = new Customer(firstName, lastName, email);
		// register
		try {
			int response = Activation.register(url + "/", jSessionId, customer);
			if (response == Activation.CREATED) {
				System.out.println("Customer registered successfully.");
				return true;
			}
		} catch (Exception e) {
			log.error("Error : " + e.getMessage(), e);
			System.out.println("Error: " + e.getMessage());
		}
		return false;
	}

	/**
	 * Fetch input provided by user.
	 */
	public String fetchUserInput() {
		Scanner in = new Scanner(System.in);

		System.out.println("Enter the HASP ID: ");
		String s = in.nextLine();
		return s;
	}

	/**
	 * This method does the following operation which are needed for activating:<br/>
	 * 1. Reads the C2V of the key.<br/>
	 * 2. Sends the C2V to the server and receives the license.</br> 
	 * 3. Updates the key with the license.	 
	 */
	protected void doV2CPActivation() throws Exception {
		String url = props.getProperty(PROP_KEY_URL);
		String c2v = null;

		System.out.println("Available HASP IDs: \n");
		displayKeyList(VENDOR_CODE);
		String haspId = fetchUserInput();
		c2v = Activation.fetchC2VforKey(haspId, VENDOR_CODE);
		boolean ispassedServer = false;
		if (c2v != null) {
			try {
				String v2c = Activation.geV2CPtLicense(url, c2v);
				ispassedServer = true;
				if (v2c != null && v2c.contains("<hasp_info>")) {
					Activation.updateKeyWithLicense(v2c, VENDOR_CODE);
					System.out.println("Key updated successfully.");
					log.info("Key updated successfully.");
				}
			} catch (Exception e) {
				if (ispassedServer) {
					log.error("Runtime Error : " + e.getMessage(), e);
					System.out.println("\nRuntime Error: " + e.getMessage());
				} else {
					log.error("EMS Error : " + e.getMessage(), e);
					System.out.println("\nEMS Error: " + e.getMessage());
				}
			}
		}
	}

	/**
	 * This method does the following operation which are needed for activating:<br/>
	 * 1. Fetch Product Key
	 * 2. Fetch machine fingerprint
	 * 3. Sends the fingerprint to the server and receives the license.</br> 
	 * 4. Updates the key with the license.
	 * 
	 */
	protected void doActivation() {
		System.out.println("Activating...");
		// get url and product key code from the properties
		String url = props.getProperty(PROP_KEY_URL);
		String pk = props.getProperty(PROP_KEY_PRODUCT_KEY);
		pk = pk.trim();
		String c2v = readC2vFromKey(VENDOR_CODE);
		boolean ispassedServer = false;
		if (c2v != null) {
			try {
				String v2c = Activation.getLicense(url, jSessionId, c2v, pk);
				ispassedServer = true;
				if (v2c.contains("<hasp_info>")) {
					Activation.updateKeyWithLicense(v2c, VENDOR_CODE);
					System.out.println("Key updated successfully.");
					log.info("Key updated successfully.");
				}
			} catch (Exception e) {
				if (ispassedServer) {
					log.error("Runtime Error : " + e.getMessage(), e);
					System.out.println("\nRuntime Error: " + e.getMessage());
				} else {
					log.error("EMS Error : " + e.getMessage(), e);
					System.out.println("\nEMS Error: " + e.getMessage());
				}
			}
		}
	}

	/**
	 * Reads and returns the key's C2V
	 * 
	 * @return the keys C2V
	 */
	protected String readC2vFromKey(String vendorCode) {
		try {
			String c2v = Activation.readC2vFromKey(vendorCode);
			if (c2v == null) {
				System.out.println("Error reading Sentinel protection key.");
				log.error("Error reading Sentinel protection key.");
			} else {
				return c2v;
			}
		} catch (Exception e) {
			log.error("Error readC2vFromKey : ", e);
			System.out.println(e.getMessage());
		}
		return null;
	}

	/**
	 * Displays available keys.
	 * 
	 * @return the keys C2V
	 */
	protected void displayKeyList(String vendorCode) {
		try {
			String keyListInfo = Activation.readListOfKeys(vendorCode);
			if (keyListInfo == null) {
				System.out
						.println("Error reading Sentinel protection key list.");
				log.error("Error reading Sentinel protection key list.");
			} else {
				System.out.println(keyListInfo);

			}
		} catch (Exception e) {
			log.error("Error readC2vFromKey : ", e);
			System.out.println(e.getMessage());
		}

	}

	/**
	 * Loads the input parameters from the properties file
	 * 
	 * @throws Exception
	 */
	protected void loadProps() throws Exception {
		InputStreamReader reader = null;
		try {
			reader = new InputStreamReader(new FileInputStream(
					"activation.properties"), "utf-8");
			props.load(reader);
		} finally {
			try {
				if (reader != null)
					reader.close();
			} catch (Exception e) {
				System.out.println(e);
				log.error("Error loading properties ", e);
			}
		}
	}

	/**
	 * Logs in with product key and sets the jsessionid and the registration
	 * required flag
	 * 
	 * @return true if login succeeded
	 * @throws Exception
	 */
	protected boolean customerLogin() throws Exception {
		System.out.println("Logging in...");
		String url = props.getProperty(PROP_KEY_URL);
		String pk = props.getProperty(PROP_KEY_PRODUCT_KEY);
		// validate input params
		if (url == null || url.equals("") || pk == null || pk.equals("")) {
			System.out
					.println("Product key and URL cannot be null. Check the activation.properties file.");
			log.info("Error Product key and URL cannot be null. Check the activation.properties file. ");
			return false;
		}
		pk = pk.trim();
		// logging in
		String[] loginRes = Activation.customerLogin(url, pk);
		// validate login response
		if (loginRes == null) {
			System.out
					.println("Login failed. No login data returned from server.");
			log.error("Error Login failed. No login data returned from server.");
			return false;
		}
		// validate and set jsessionid and registration flag
		jSessionId = loginRes[0];
		if (jSessionId == null) {
			System.out.println("Login failed. Session not created.");
			log.error("Error Login failed. Session not created");
			return false;
		}

		if (loginRes[1] != null) {
			registrationStat = Integer.parseInt(loginRes[1]);
		}
		if (loginRes[2] != null) {
			redirectToUserReg = loginRes[2];
		}
		System.out.println("You were logged in successfully.");
		return true;
	}

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		ActivationSample activationSample = new ActivationSample();
		activationSample.activate();
	}

}

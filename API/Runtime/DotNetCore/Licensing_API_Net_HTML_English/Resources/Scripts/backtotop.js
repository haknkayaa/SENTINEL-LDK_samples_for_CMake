// This script includes:
//      - back-to-top
//      - search functionality
//      - resizing for TOC navigation sidebar

var isResize=false;
var firstTry = true;
$(document).ready(function () {
	addDragHandle(); // add Drag element
	//
	
		//
	resize(); // add events for dragging
    
	if ($(".body-container")) {
        $(".body-container").on('scroll', function () {
            scrollEle(this)
        });
    }

    $(window).on('scroll', function () {
        scrollEle(this)
    });
    function scrollEle(ele) {
        if ($(ele).scrollTop() > 20)
            $('.backToTop').fadeTo("slow", 1);
        else 
            $('.backToTop').hide();
    }
	
//	createNavBar(); // calling Search navigation functionality
	
	
})
      
			// back-to-top ends here
			// Close button ("^ Collapse") for dropdowns and togglers starts here. 
      // The styling is done via the CSS (a.dropDownClose, a.togglerClose)
			$(document).ready(function(){
 
				/* Add collapse button to dropdowns */
				$(".dropDownBody").append("<a href='javascript:void(0);' class='dropDownClose'>Collapse</a>");
				$(".dropDownClose").click(function() {
						$(this).parent().prev(".dropDownHead").children(".dropDownHotspot").click();
					});
 
				
    // Search functionality starts here
    // search navigation - function call starts here
			createNavBar(); 
			})
		// ***Search navigation code starts here***
		var myVar;
		function createNavBar() {
			sessionStorage.setItem('navAttempt', 1);

			myVar = setInterval(createNav, 1000); // Create a loop to check for search items after a one second gap (because search highlights are not instantly visible after the page loads).
		}

		function createNav()       // Function that adds Search Nav to page
		{
			var ele = document.querySelectorAll(".SearchHighlight");    // Get the list of all highlighted text on the page
			if (ele.length != 0) {
				clearInterval(myVar); // Disable the loop because we have search results
				ele[0].scrollIntoView({ block: "center" }); // Scroll to first highlighted text
				setSearchValue(ele[0]);
				if (ele.length > 1) {   // If there is more than one instance of the highlighted text, then display search navigation
					var location = 1;
					sessionStorage.setItem('searchLocation', location);
					sessionStorage.setItem("bgColor", ele[0].style.backgroundColor)
					var div = document.createElement("div");
					div.setAttribute("id", "searchResultBox");
					div.className = "search-result-box";
					var span = document.createElement("span");
					span.textContent = ele.length + " Results Found";
					span.className = "result-text";
					var b = document.createElement("span");
					b.className = "search-anchors";
					var prevAnchor = document.createElement("a");
					prevAnchor.setAttribute("onclick", "previousSearchOption()");
					prevAnchor.className = "search-previous";
					var nextAnchor = document.createElement("a");
					nextAnchor.setAttribute("onclick", "nextSearchOption()");
					nextAnchor.className = "search-next";
					var label = document.createElement("label");
					label.textContent = location + "/" + ele.length;
					label.setAttribute("id", "searchCounter")
					label.className = "search-counter";

					b.appendChild(prevAnchor);
					b.appendChild(label);
					b.appendChild(nextAnchor);
					var divider = document.createElement("span");
					divider.className = "search-divider";
					div.appendChild(span);
					div.appendChild(divider);
					div.appendChild(b)
					var searchBar = document.querySelector("div.nav-search-wrapper")
					searchBar.insertBefore(div, searchBar.childNodes[0]);
					ele[0].style.backgroundColor = "#ff9632";
			
					//Adding event listener
					document.querySelector(".button.remove-highlight-button").addEventListener("click", function(){ 
							var searchBox = document.getElementById("searchResultBox");
							if(searchBox != null)
							{
								searchBox.parentNode.removeChild(searchBox);
							}
						});
				}
        
			}
			else {
				var navAttempt = parseInt(sessionStorage.getItem("navAttempt"));
				if (navAttempt > 8) {
					clearInterval(myVar);// disabling the loop after 4 seconds and search highlight is not on screen
				}
				else {
					navAttempt = navAttempt + 1;
					sessionStorage.setItem('navAttempt', navAttempt);
				}
			}

		}

		function setSearchValue(ele)
		{
			var url = window.location.href;
			var result = new Object();
			var idx = url.lastIndexOf('?');
			if (idx > 0)
			{
				var params = url.substring(idx + 1).split('&');
		
				for (var i = 0; i < params.length; i++)
				{
					idx = params[i].indexOf('=');
			
					if (idx > 0)
					{
						result[params[i].substring(0, idx)] = params[i].substring(idx + 1);
					}
				}
			}
			if(result['highlight'] != null)
			{
				document.querySelector("input.search-field").value = decodeURI(result['highlight']);
			}
			else if(result['Highlight'] != null)
			{
				document.querySelector("input.search-field").value = decodeURI(result['Highlight']);
			}
			else
			{
				document.querySelector("input.search-field").value = ele.textContent;
			
			}
	
		}
		function previousSearchOption()   // Function when we click on previous in search navigation
		{
			var location = parseInt(sessionStorage.getItem('searchLocation'));
			var bgColor = sessionStorage.getItem("bgColor");
			var ele = document.querySelectorAll(".SearchHighlight");
			ele[location - 1].style.backgroundColor = bgColor;
			location = location - 1;
			if (location == 0) {
				location = ele.length;
			}

			ele[location - 1].scrollIntoView({ block: "center" });
			ele[location - 1].style.backgroundColor = "#ff9632";
			sessionStorage.setItem('searchLocation', location);
			document.getElementById("searchCounter").textContent = location + "/" + ele.length;



		}
		function nextSearchOption()  // Function when we click on next in search navigation
		{
			var location = parseInt(sessionStorage.getItem('searchLocation'));
			var bgColor = sessionStorage.getItem("bgColor");
			var ele = document.querySelectorAll(".SearchHighlight");
			ele[location - 1].style.backgroundColor = bgColor;
			location = location + 1;
			if (location > ele.length) {
				location = 1;
			}

			ele[location - 1].scrollIntoView({ block: "center" });
			ele[location - 1].style.backgroundColor = "#ff9632";
			window.scrollBy(0, -10);
			sessionStorage.setItem('searchLocation', location);
			document.getElementById("searchCounter").textContent = location + "/" + ele.length;



		}
	// ***Search navigation code ends here***
    // Search functionality ends here

// Make navigation sidebar resizable by 50%
function addDragHandle()
{
	
	var drag=document.createElement("div");
	drag.setAttribute("id","draghandle");
	drag.className="draghandle";
	var container=document.querySelector(".row.outer-row");
	container.insertBefore(drag,container.childNodes[3]);
	width=localStorage.getItem('sidebarWidth');
	if(width!=null)
	{
		left = $('.sidenav-wrapper');
		left.css('width', width);
	}
	
}

function resize() {
    var container = $('.main-section'),
        left = $('.sidenav-wrapper'),
        handle = $('.draghandle');
	
    handle.on('mousedown', function (e) {
        isResize = true;
        lastDownX = e.clientX;
        console.log('mouseDown');
    });
	
    $(document).on('mousemove', function (e) {
        // we don't want to do anything if we aren't resizing.
        if (!isResize) 
            return;
        maxWidth=(container.width()*20)/100;
		min=maxWidth/2;
		var offsetRight = null;
		if (firstTry){
			offsetRight = container.width() - (container.width() - maxWidth);
			firstTry = false;
		}
		else
			offsetRight = container.width() - (container.width() - e.clientX);
		//
		if(offsetRight>maxWidth)
		{
			offsetRight=maxWidth;
		}
		if(offsetRight<min)
		{
			offsetRight=min;
		}
		left.css('width', offsetRight);
        localStorage.setItem('sidebarWidth', offsetRight);
        e.preventDefault();
		//right.css('width', offsetRight);
    });
	$(document).on('mouseup', function (e) {
        // stop resizing
        isResize = false;
    });
}
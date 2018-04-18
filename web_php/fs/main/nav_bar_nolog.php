<link href="/web_bcg/fs/css/style.css" rel="stylesheet">

<div class="htmleaf-container" style="margin-left:10%;width:80%;margin-top: 80px;">
	<nav class="animenu"> 
	  <button class="animenu__toggle">
		<span class="animenu__toggle__bar"></span>
		<span class="animenu__toggle__bar"></span>
		<span class="animenu__toggle__bar"></span>
	  </button>
	  <ul class="animenu__nav">
		<li>
		  <a href="/web_bcg/fs/main/mainindex.php">主页</a>
		</li>
	
		
			
		<li>		
		 <a href="/web_bcg/fs/main/help.php?srcflg=0" >帮助</a>	
		</li>
	  </ul>
	</nav>
</div>
<div style="border-top:solid 1px rgb(17,57,101);width:100%;height:20px;bottom:0px;
	position:fixed;text-align:center;font-size:12px;padding-top: 3px;left: 0px;background-color:#F0F0F0;">
<a>www.waqrs.com 我爱嵌入式 Copyright ©2016 QQ:448251593 </a> 
</div>
<script type="text/javascript">
	(function(){
	  var animenuToggle = document.querySelector('.animenu__toggle'),
		  animenuNav    = document.querySelector('.animenu__nav'),
		  hasClass = function( elem, className ) {
			return new RegExp( ' ' + className + ' ' ).test( ' ' + elem.className + ' ' );
		  },
		  toggleClass = function( elem, className ) {
			var newClass = ' ' + elem.className.replace( /[\t\r\n]/g, ' ' ) + ' ';
			if( hasClass(elem, className ) ) {
			  while( newClass.indexOf( ' ' + className + ' ' ) >= 0 ) {
				newClass = newClass.replace( ' ' + className + ' ' , ' ' );
			  }
			  elem.className = newClass.replace( /^\s+|\s+$/g, '' );
			} else {
			  elem.className += ' ' + className;
			}
		  },           
		  animenuToggleNav =  function (){        
			toggleClass(animenuToggle, "animenu__toggle--active");
			toggleClass(animenuNav, "animenu__nav--open");        
		  }

	  if (!animenuToggle.addEventListener) {
		  animenuToggle.attachEvent("onclick", animenuToggleNav);
	  }
	  else {
		  animenuToggle.addEventListener('click', animenuToggleNav);
	  }
	})()
</script>
<script>
var _hmt = _hmt || [];
(function() {
  var hm = document.createElement("script");
  hm.src = "//hm.baidu.com/hm.js?7a34a7320a12094b37f5ddb5e8b02814";
  var s = document.getElementsByTagName("script")[0]; 
  s.parentNode.insertBefore(hm, s);
})();
</script>

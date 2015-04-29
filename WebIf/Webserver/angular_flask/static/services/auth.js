'use strict';

angular.module('wateringApp')
.factory('Auth', [ '$http', '$rootScope', '$window', 'Session', 'AUTH_EVENTS', 
function($http, $rootScope, $window, Session, AUTH_EVENTS) {
	var authService = {};
	
	
	//the login function
	authService.login = function(user, success, error) {
        
        
        $http.get('api/user').success(function(response) {
            var users = response.objects;
            var loginData = {};
            
            // check all passwords from db
            for(var i=0; i < users.length; i++) {
                loginData.user = 'admin';
                loginData.password = users[i].password;
                loginData.userRole = 'admin';
                
                //var encrypted = '' + CryptoJS.AES.encrypt(loginData.password, "#WaterCtrlS3cret");
                //var decrypted = CryptoJS.AES.decrypt(encryptedencrypted, "#WaterCtrlS3cret");
                
                var decryptedPassword = CryptoJS.AES.decrypt(loginData.password, "#WaterCtrlS3cret").toString(CryptoJS.enc.Utf8);
             
                // password is correct
                if(user.password == decryptedPassword) {
                    //set the browser session, to avoid relogin on refresh
                    $window.sessionStorage["userInfo"] = JSON.stringify(loginData);

                    //delete password not to be seen clientside 
                    delete loginData.password;

                    //update current user into the Session service or $rootScope.currentUser
                    //whatever you prefer
                    Session.create(loginData);
                    //or
                    $rootScope.currentUser = loginData;

                    //fire event of successful login
                    $rootScope.$broadcast(AUTH_EVENTS.loginSuccess);
                    //run success function
                    success(loginData);
                }
            }
            
            if(!authService.isAuthenticated) {
                //unsuccessful login, fire login failed event for 
                //the according functions to run
                $rootScope.$broadcast(AUTH_EVENTS.loginFailed);
                error();
            }
            
            
        });
        
        
		
		
	};

	//check if the user is authenticated
	authService.isAuthenticated = function() {
		return !!Session.user;
	};
	
	//check if the user is authorized to access the next route
	//this function can be also used on element level
	//e.g. <p ng-if="isAuthorized(authorizedRoles)">show this only to admins</p>
	authService.isAuthorized = function(authorizedRoles) {
		if (!angular.isArray(authorizedRoles)) {
	      authorizedRoles = [authorizedRoles];
	    }
	    return (authService.isAuthenticated() &&
	      authorizedRoles.indexOf(Session.userRole) !== -1);
	};
	
	//log out the user and broadcast the logoutSuccess event
	authService.logout = function(){
		Session.destroy();
		$window.sessionStorage.removeItem("userInfo");
		$rootScope.$broadcast(AUTH_EVENTS.logoutSuccess);
	}

	return authService;
} ]);
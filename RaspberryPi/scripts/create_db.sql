CREATE DATABASE waterctrl;
CREATE USER 'waterctrl_user'@'localhost' IDENTIFIED BY 'Wfmd22nGixsP';
GRANT ALL ON waterctrl.* TO 'waterctrl_user'@'localhost';
FLUSH PRIVILEGES;

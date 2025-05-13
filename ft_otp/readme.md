# FT_OTP

42 school project that aims tom implement TOTP algorithm on our own

## Commands

 - install oathtool to test the project :

    ```bash ./install_oath.sh```

    ```source ~/.bashrc```
    
 - launch test with oathtool:

    ``` oathtool --totp $(cat key.hex)``` 
    
 - encrypt the key:

    ```./ft_otp -g key.hex```
    
 - generate TOTP:
 
   ```./ft_otp -k ft_otp.key```

## Glossary

 - **OTP:**  one-time password
 - **TOTP:** Time-based one-time password
 - **HOTP** Hash-based one-time password

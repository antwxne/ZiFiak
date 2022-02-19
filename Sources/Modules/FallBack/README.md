##Module: Fall Back

####Description:

- This module is use to handle response error

####Config:

- This module does not need any configuration

####How it works ?

- In case of error during your module's execution, you can setup a ErrorOccured variable in the Context, with a pair of response code and std::string (for error message), if the ErrorOccured variable is present this module with setup response with status code and body message.
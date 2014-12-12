package spbau.novokreshchenov.network.hw1.Exceptions;

import com.sun.jersey.api.MessageException;


public class BankNotFoundException extends MessageException {
    private final String bankName;

    public BankNotFoundException(String bankName) {
        super("Bank with specified name=\'" + bankName + "\' hasn't been found");
        this.bankName = bankName;
    }

    public String getBankName() {
        return bankName;
    }
}

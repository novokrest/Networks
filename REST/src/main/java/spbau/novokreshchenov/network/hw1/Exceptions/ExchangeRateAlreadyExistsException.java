package spbau.novokreshchenov.network.hw1.Exceptions;

import com.sun.jersey.api.MessageException;


public class ExchangeRateAlreadyExistsException extends MessageException {
    private final String bankName;
    private final String currency;

    public ExchangeRateAlreadyExistsException(String bankName, String currency) {
        super("Bank \'" + bankName + "\' already has information about \'" + currency +"\' exchange rate");
        this.bankName = bankName;
        this.currency = currency;
    }

    public String getBankName() {
        return bankName;
    }

    public String getCurrency() {
        return currency;
    }
}

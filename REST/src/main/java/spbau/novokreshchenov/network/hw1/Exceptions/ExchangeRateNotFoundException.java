package spbau.novokreshchenov.network.hw1.Exceptions;

import com.sun.jersey.api.MessageException;


public class ExchangeRateNotFoundException extends MessageException {
    private final String bankName;
    private final String currency;

    public ExchangeRateNotFoundException(String bankName, String currency) {
        super("Bank \'" + bankName + "\' doesn't has information about \'" + currency + "\' exchange rate");
        this.bankName = bankName;
        this.currency = currency;
    }
}

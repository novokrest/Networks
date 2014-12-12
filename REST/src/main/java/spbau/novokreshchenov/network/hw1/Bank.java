package spbau.novokreshchenov.network.hw1;


import spbau.novokreshchenov.network.hw1.Exceptions.ExchangeRateAlreadyExistsException;
import spbau.novokreshchenov.network.hw1.Exceptions.ExchangeRateNotFoundException;

import java.util.HashMap;
import java.util.Map;

public class Bank {
    private final String name;
    private final String iconPath;
    private final Map<String, ExchangeRate> exchangeRate = new HashMap<String, ExchangeRate>();

    public Bank(String name, String iconPath) {
        this.name = name;
        this.iconPath = iconPath;
    }

    public String getName() {
        return name;
    }

    public void putExchangeRate(String currency, float buyValue, float sellValue) {
        exchangeRate.put(currency, new ExchangeRate(buyValue, sellValue));
    }

    public void addExchangeRate(String currency, float buyValue, float sellValue) throws ExchangeRateAlreadyExistsException, ExchangeRateNotFoundException {
        if (!hasExchangeRate(currency)) {
            putExchangeRate(currency, buyValue, sellValue);
        }

        throw new ExchangeRateAlreadyExistsException(name, currency);
    }

    public void updateExchangeRate(String currency, float buyValue, float sellValue) throws ExchangeRateNotFoundException {
        if (hasExchangeRate(currency)) {
            putExchangeRate(currency, buyValue, sellValue);
        }

        throw new ExchangeRateNotFoundException(name, currency);
    }

    public ExchangeRate getExchangeRate(String currency) throws ExchangeRateNotFoundException {
        if (hasExchangeRate(currency)) {
            return exchangeRate.get(currency);
        }

        throw new ExchangeRateNotFoundException(name, currency);
    }

    public void removeExchangeRate(String currency) throws ExchangeRateNotFoundException {
        if (hasExchangeRate(currency)) {
            exchangeRate.remove(currency);
        }

        throw new ExchangeRateNotFoundException(name, currency);
    }

    public boolean hasExchangeRate(String currency) {
        return exchangeRate.containsKey(currency);
    }

    public String getIconPath() {
        return iconPath;
    }
}

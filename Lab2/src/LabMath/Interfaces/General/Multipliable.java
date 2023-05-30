package LabMath.Interfaces.General;

import jdk.jshell.spi.ExecutionControl;

public interface Multipliable<T> {
    void mul(T other) throws ExecutionControl.NotImplementedException;
}

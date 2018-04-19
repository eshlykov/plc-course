//'main' method must be in a class 'Rextester'.
//Compiler version 1.8.0_111

import java.util.*;
import java.lang.*;

class Rextester {

    interface ITextHandler {
        String Process(String text);
    };

    static class LowercaseHandler implements ITextHandler {
        public String Process(String token) {
            return token.toLowerCase();
        }
    };

    static class HandlersArray<T extends ITextHandler> {
        private static final int initialCapacity = 4;
        private T[] buffer;
        private int size = 0;
        private int capacity = initialCapacity;

        public HandlersArray() {
            buffer = (T[]) new ITextHandler[capacity];
        }

        public void add(T element) {
            buffer[size] = element;
            ++size;
            tryResize();
        }

        public int size() {
            return size;
        }

        public T get(int index) {
            return buffer[index];
        }

        public String apply(String text) {
            String accumulator = text;
            for( int i = 0; i < size; ++i ) {
                accumulator = buffer[i].Process( accumulator );
            }
            return accumulator;
        }

        private void tryResize() {
            if( size != capacity ) {
                return;
            }

            int newCapacity = 2 * capacity;
            T[] newBuffer = (T[]) new ITextHandler[newCapacity];

            for( int i = 0; i < capacity; ++i ) {
                newBuffer[i] = buffer[i];
            }

            capacity = newCapacity;
            buffer = newBuffer;
        }
    }

    public static void main(String[] args)
    {
        HandlersArray<ITextHandler> handlers = new HandlersArray<ITextHandler>();
        handlers.add( new LowercaseHandler() );
        System.out.println(handlers.apply( "SaMple teXt" ));
    }
}
